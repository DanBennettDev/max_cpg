/// @file	
///	@ingroup 	CPG
/// @author		Dan Bennett
///	@license	Usage of this file and its contents is governed by the MIT License


/*

	TODO:
		- OSC front end
		- Handle DSP stop/start


	LONGER TERM:
		Invertable connections
		Smoothing control changes
		Sync to max internal timings
		External input(s)
		Waveshaping between connections (raise to power and threshold)

		Per node quantiser amount setting
		Tool for converting network params back into a a network graph for tweaking (ML?)

		PERFORMANCE:
			switchable triggering
			Curve lookup every sample? (for freq check)
			Look for other bottlenecks
			setting root freq is SLOOW
			why is higher freq more demanding on CPU?

			audio version: no need to sync children


*/



#include "c74_min.h"
#include "matsuokaEngine.h"
#include "matsuNode.h"


#define ARGS_BEFORE_PARAMS 5
#define CALIBRATION_CYCLES 20
#define TRIGGER_WIDTH 0.02

#define MAX_NODES 16
#define INTERP_SAMPLES 4
#define UNITY_CONN_WEIGHT 3
#define P_TRATIO 4
#define P_C 1
#define P_B 7
#define P_G 7
#define P_COMPENSATION 0.973200f
#define DEFAULT_CURVE_X 0.25f, 0.333f, 0.5f, 1.111f, 1.333f, 2.f, 3.f, 4.f, 6.f, 8.f
#define DEFAULT_CURVE_Y 1.955492228f, 1.098290155f, 0.107150259f, 0.133937824f, 0.321450777f, 0.517f, 0.641026425f, 0.937029016f, 1.194725389f, 1.259015544f
#define FLOAT_EPSILON 0.00001f
#define SYNC_CHANNEL_OFFSET 1
// more than adequate resolution for rhythm sequencing
#define DEFAULT_SAMPLE_RATE 1000	



using namespace c74::min;

class cpg_net : public object<cpg_net>, public vector_operator<>
{
private:
	class _ramp {
	public:
		_ramp() {
			setLength((int)(TRIGGER_WIDTH * 44100));
		}

		float tick() {
			_phase += _phaseStep;
			if (_phase < 1) {
				return 1;
			}
			return 0;
		}



		float phase() {
			return _phase;
		}

		void setTrigger() {
			_phase = 0;
		}

		void setLength(int samples) {
			_length = samples;
			_phaseStep = 1.f / _length;
		}

	private:
		int _length;
		float _phase{ 0 };
		float _phaseStep;
	};

	using externalSync = MatsuokaEngine::externalSync;

	std::shared_ptr<MatsuokaEngine> _engine_ptr;
	MatsuNode _dummyNode;
	double _freqComp{ P_COMPENSATION };
	int _local_srate{ DEFAULT_SAMPLE_RATE };
	bool _send_noteTriggers{ true };
	bool _externalInputs{ false };
	externalSync _syncInput{ externalSync::none };
	int _syncInputChannel{ -1 };
	float _prevDrivingInput{ 0 };

	// holds raw output values for interpolation. 
	DelayLine<float> _outRingBuff[MAX_NODES];
	int _ringIndex{ 0 };
	float _freqs[MAX_NODES]{ 1 };
	number _phase{ 0 };
	number _phaseStep{ 0 };
	_ramp _trigs[MAX_NODES];

	lib::interpolator::hermite<sample> _interp_herm;
	lib::interpolator::linear<sample> _interp_lin;
	bool _initialized{ false };
	int _nodeCount{ 2  };	// defaults to 1 channel / 1 node

	vector< unique_ptr<inlet<>> >			_ins;				///< this object's ins
	vector< unique_ptr<outlet<>> >			_outs;				///< this object's outs

public:


	MIN_DESCRIPTION{ "A network of neural oscillators designed for sequencing / control signal use" };
	MIN_TAGS{ "audio, oscillator, LFO, control, sequencing" };
	MIN_AUTHOR{ "Cycling '74" };
	MIN_RELATED{ "phasor~" };






	// Because our object defines a constructor (below) these argument definitions are for
	// documentation purposes only.
	argument<int> node_count_arg{ this, "node_count", "The number of nodes in the network." };
	argument<int> sample_rate_arg{ this, "sample_rate", "The sample rate of network calculations." };
	argument<int> trigger_outputs_arg{ this, "trigger_outputs", "1(default) object has signal and note trigger outputs, 0: object has only signal outputs" };
	argument<int> ext_inputs_arg{ this, "external_signal_inputs", "0 (default) object has external signal inputs, 0: object has none" };
	argument<int> sync_input_arg{ this, "sync_input", "0 (default) object has sync input, 0: object has none" };
	argument<int> tRatio_arg{ this, "tRatio_param", "The ratio of equation params t2:t1." };
	argument<int> c_arg{ this, "c_param", "The equation parameter c." };
	argument<int> b_arg{ this, "b_param", "The equation parameter b." };
	argument<int> g_arg{ this, "g_param", "The equation parameter g." };



	cpg_net(const atoms& args = {})
	{
		_initialized = false;

		cout << "started" << endl;
		for (int i = 0; i < MAX_NODES; i++)
			_outRingBuff[i].resize(INTERP_SAMPLES);


		// gather info for basic object init
		if (args.size() > 0) {
			_nodeCount = args[0];
		}

		if (args.size() > 1) {
			unsigned srate_in = (unsigned)(int)args[1];
			cout << "sample rate requested: " << srate_in << endl;
			if ((int)srate_in > samplerate() / 2 || (int)srate_in < 0) {
				cout << "sample rate must be system sample rate, or else >0 and < system sample-rate / 2: " << srate_in << endl;
				cout << "setting to system sample rate" << endl;

				_local_srate = (int)samplerate();

			}
			else {
				_local_srate = srate_in;
			}
		}
		else {
			_local_srate = (int)samplerate();
		}

		if (args.size() > 2) {
			_send_noteTriggers = args[2] == 1 ? true : false;
		}

		if (args.size() > 3) {
			_externalInputs = args[3] == 1 ? true : false;
		}
		if (args.size() > 4) {
			switch ((int)args[4]) {
			case 0:
				_syncInput = externalSync::none;
				break;
			case 1:
				_syncInput = externalSync::driving;
				break;
			case 2:
				_syncInput = externalSync::reseting;
				break;
			default:
				_syncInput = externalSync::none;
				break;
			}
		}

		_engine_ptr = std::shared_ptr<MatsuokaEngine>(
			new  MatsuokaEngine(_local_srate, _send_noteTriggers, false, false));
		_dummyNode = MatsuNode();

		// set up nodes and ins/outs for them
		for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) {
			_engine_ptr->setNodeQuantiser_Grid(nodeID, MatsuokaEngine::gridType::unQuantised);
			_ins.push_back(std::make_unique<inlet<>>(this, "(signal) freq input " + nodeID));
			_outs.push_back(std::make_unique<outlet<>>(this, "(signal) signal output " + nodeID, "signal"));
			_trigs[nodeID].setLength((int)(TRIGGER_WIDTH * _local_srate));

			if (nodeID != 0) {
				_engine_ptr->addChild(0, nodeID);
			}
		}

		if (_send_noteTriggers) {
			for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) 
				_outs.push_back(std::make_unique<outlet<>>(this, "(signal) trigger output " + nodeID, "signal"));
		}
		if (_externalInputs) {
			for (int nodeID = 0; nodeID < _nodeCount; ++nodeID)
				_ins.push_back(std::make_unique<inlet<>>(this, "(signal) signal input " + nodeID, "signal"));
		}


		vector<float> curvX = { DEFAULT_CURVE_X };
		vector<float> curvY = { DEFAULT_CURVE_Y };


		if (_syncInput != externalSync::none) {
			_ins.push_back(std::make_unique<inlet<>>(this, "(signal) sync input "));
		}

		_engine_ptr->loadConnectionWeightCurve(curvX, curvY);
		_engine_ptr->setUnityConnectionWeight(UNITY_CONN_WEIGHT);
		_engine_ptr->setConnectionWeightScaling(true);
		_engine_ptr->setFreqCompensation(P_COMPENSATION);
		setParams(args, true);


		// set frequencies and connect all nodes to all others, but with 0 signal weight
		for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) {
			//set all frequencies to 1
			_engine_ptr->setNodeFrequency(nodeID, 1.0, false);
			for (int connectToID = 0; connectToID < _nodeCount; ++connectToID) {
				if (nodeID != connectToID) {
					_engine_ptr->setConnection(nodeID, connectToID, 0.0);
				}
			}
		}

		_engine_ptr->doQueuedActions();
		calibrate.set();
		if (_syncInput != externalSync::none) {
			_engine_ptr->setDriven(_syncInput);
			_engine_ptr->setDrivingInput(0);
		}

		cout << "initialised network" << endl;

		_initialized = true;
	}



	//argument<number> a_t1 { this, "t1", "equation parameter t1",
	//	MIN_ARGUMENT_FUNCTION {
	//		node.set_t1( arg);
	//	}
	//};


	message<threadsafe::yes> bang{ this, "bang", "reset the node",
		MIN_FUNCTION{
		if (_initialized) {
			_engine_ptr->reset();
		}
	return {};
	}
	};


	message<threadsafe::yes> number{ this, "number", "Set frequency.",
		MIN_FUNCTION{
			if (!args.empty()) {
				setFreq(inlet, args[0]);
			}
		return {};
	}
	};

	message<threadsafe::yes> params{ this, "params",
		MIN_FUNCTION{
		setParams(args, false);
		calibrate.set();
	return {};
	}
	};


	message<threadsafe::yes> weight{ this, "weight",
		MIN_FUNCTION{
		if (args.size() >= 3 && _engine_ptr->nodeExists((int)args[0]) && _engine_ptr->nodeExists((int)args[1])) {
			_engine_ptr->setConnection((int)args[0], (int)args[1], (double)args[2]);
		}
	return {};
	}
	};

	message<threadsafe::yes> offset_conn{ this, "offset_conn",
		MIN_FUNCTION{
		if (args.size() >= 3 && _engine_ptr->nodeExists((int)args[0]) && _engine_ptr->nodeExists((int)args[1])) {
			_engine_ptr->setConnectionPhaseOffset((int)args[0], (int)args[1], (double)args[2]);
		}
	return {};
	}
	};

	message<threadsafe::yes> offset_out{ this, "offset_out",
		MIN_FUNCTION{
		if (args.size() >= 2 && _engine_ptr->nodeExists((int)args[0])) {
			_engine_ptr->setNodePhaseOffset((int)args[0],(double)args[1]);
		}
	return {};
	}
	};

	message<threadsafe::yes> noise{ this, "noise",
		MIN_FUNCTION{
		if (args.size() >= 3 && _engine_ptr->nodeExists((int)args[0])) {
			_engine_ptr->setNodeSelfNoise((int)args[0], (double)args[1]);
		}
	return {};
	}
	};


	message<threadsafe::yes> quant_grid{ this, "quant_grid",
		MIN_FUNCTION{
		using gridType = QuantisedEventQueue::gridType;
		if (args.size() >= 2 && _engine_ptr->nodeExists((int)args[0])) {
			if (args[1] == "none") {
				_engine_ptr->setNodeQuantiser_Grid((int)args[0], gridType::unQuantised);
				cout << "node " << (int)args[0] << " set to unquantised" <<endl;

			}
			else  if (args[1] == 24) {
				cout << "node " << (int)args[0] << " set to grid size 24" << endl;
				_engine_ptr->setNodeQuantiser_Grid((int)args[0], gridType::_24th);
			}
			else  if (args[1] == 32) {
				cout << "node " << (int)args[0] << " set to grid size 32" << endl;
				_engine_ptr->setNodeQuantiser_Grid((int)args[0], gridType::_32nd);
			}
			else {
				cout << "invalid quantiser grid" << endl;
			}
		}
	return {};
	}
	};

	message<threadsafe::yes> quant_mult{ this, "quant_mult",
		MIN_FUNCTION{
			if (args.size() >= 2 && _engine_ptr->nodeExists((int)args[0])) {
				_engine_ptr->setNodeQuantiser_Multiple((int)args[0], (float)args[1]);
			}
	return {};
	}
	};

	message<threadsafe::yes> quant_offset{ this, "quant_offset",
		MIN_FUNCTION{
		if (args.size() >= 2 && _engine_ptr->nodeExists((int)args[0])) {

				_engine_ptr->setNodeQuantiser_Offset((int)args[0], (float)args[1]);
		}
	return {};
	}
	};

	message<threadsafe::yes> quant_amount{ this, "quant_amount",
		MIN_FUNCTION{
		if (args.size() >= 1) {
			_engine_ptr->setQuantiseAmount((float)args[0]);
		}
	return {};
	}
	};

	message<threadsafe::yes> quant_amount_node{ this, "quant_amount_node",
		MIN_FUNCTION{
		if (args.size() >= 2 && _engine_ptr->nodeExists((int)args[0])) {
			_engine_ptr->setQuantiseAmount((int)args[0], (float)args[1]);
		}

	return {};
	}
	};


	queue<> calibrate{ this,
		MIN_FUNCTION{
		doCalibration();
	return {};
	}
	};



	//attribute<number> frequency { this, "frequency" , 1.0, description{"Frequency in Hz"},
	//	setter { MIN_FUNCTION {
	//		if (_initialized) {
	//			m_oscillator.frequency(args[0], samplerate());
	//		}
	//		return args;
	//	}}
	//};




	void calcVector_nonInterp(audio_bundle input, audio_bundle output)
	{
		int syncInputNo = _nodeCount * (_externalInputs ? 2 : 1);
		// For each frame in the vector calc each channel
		for (auto frame = 0; frame<input.frame_count(); ++frame) {
			if (_syncInput == externalSync::driving) {
				_engine_ptr->setDrivingInput((float)input.samples(syncInputNo)[frame]);
			}
			else if (_syncInput == externalSync::reseting) {
				float drivingInput = (float)input.samples(syncInputNo)[frame];
				if (drivingInput < _prevDrivingInput) {
					_engine_ptr->zeroSync(0);
				}
				_prevDrivingInput = (float)input.samples(syncInputNo)[frame];
			}
			if (_externalInputs) {
				for (int channel = 0; channel < _nodeCount; ++channel) {
					_engine_ptr->setNodeExternalInput(channel, 1, (float)input.samples(channel + _nodeCount)[frame]);
				}
			}

			_engine_ptr->step();
			
			// signals
			for (int channel = 0; channel < _nodeCount; ++channel) {
				// send to max output
				if (_ins[channel]->has_signal_connection()){
					setFreq(channel, (float)input.samples(channel)[frame]);
				}
				_outRingBuff[channel].pushSample((float)_engine_ptr->getNodeOutput(channel,0, _send_noteTriggers));
				output.samples(channel)[frame] = _outRingBuff[channel].getDelayed(0);
			}
			if (_send_noteTriggers) {
				auto noteEvents = _engine_ptr->getEvents();
				for each (auto note in noteEvents) {
					_trigs[note.nodeID].setTrigger();
				}
				for (int channel = 0; channel < _nodeCount; channel++) {
					output.samples(channel + _nodeCount)[frame] = _trigs[channel].tick();
				}
			}
		}
	}


	void calcVector_interp(audio_bundle input, audio_bundle output)
	{
		int syncInputNo = _nodeCount * (_externalInputs ? 2 : 1);
		// For each frame in the vector calc each channel
		for (auto frame = 0; frame<input.frame_count(); ++frame) {
			if (_syncInput == externalSync::driving) {
				_engine_ptr->setDrivingInput((float)input.samples(syncInputNo)[frame]);
			}
			else if (_syncInput == externalSync::reseting) {
				float drivingInput = (float)input.samples(syncInputNo)[frame];
				if (drivingInput < _prevDrivingInput) {
					_engine_ptr->zeroSync(0);
				}
				_prevDrivingInput = (float)input.samples(syncInputNo)[frame];
			}
			// if _phase wraps, calc sample from network
			_phase += _phaseStep;
			if (_phase > 1.0) {
				_phase -= 1.0;
				if (_externalInputs) {
					for (int channel = 0; channel < _nodeCount; ++channel) {
						_engine_ptr->setNodeExternalInput(channel, 1, (float)input.samples(channel + _nodeCount)[frame]);
					}
				}
				_engine_ptr->step();

				if (_send_noteTriggers) {
					auto noteEvents = _engine_ptr->getEvents();
					for each (auto note in noteEvents) {
						_trigs[note.nodeID].setTrigger();
					}
					for (int channel = 0; channel < _nodeCount; channel++) {
						_trigs[channel].tick();
					}
				}

			}
			else if (_phase < 0.0) { // sholdn't happen
				_phase += 1.0;
			}

			// regardless of _phase wrap, calculate our interpolated sample for outputs

			for (int channel = 0; channel < _nodeCount; ++channel) {
				
				if (_ins[channel]->has_signal_connection()) {
					setFreq(channel, (float)input.samples(channel)[frame]);
				}
				_outRingBuff[channel].pushSample((float)_engine_ptr->getNodeOutput(channel, 0, _send_noteTriggers));

				if (_local_srate > 11024) {
					output.samples(channel)[frame] =
						_interp_herm(	_outRingBuff[channel].getDelayed(3),
										_outRingBuff[channel].getDelayed(2),
										_outRingBuff[channel].getDelayed(1),
										_outRingBuff[channel].getDelayed(0),
										_phase);
				}
				else {
					output.samples(channel)[frame] =
						_interp_lin(_outRingBuff[channel].getDelayed(3),
							_outRingBuff[channel].getDelayed(2),
							_outRingBuff[channel].getDelayed(1),
							_outRingBuff[channel].getDelayed(0),
							_phase);
				}

				if (_send_noteTriggers) {
					output.samples(channel + _nodeCount)[frame] = _trigs[channel].phase() < 1 ? 1 : 0;
				}
			}
		}
	}



	/// Process N channels of audio
	/// Max takes care of squashing denormal for us by setting the FTZ bit on the CPU.
	void operator()(audio_bundle input, audio_bundle output) 
	{
		if (_initialized) {
			_engine_ptr->doQueuedActions();
			_phaseStep = _local_srate / samplerate();

			if (_local_srate == (int)samplerate()) {
				calcVector_nonInterp(input, output);
			}
			else {
				calcVector_interp(input, output);
			}
		}

	}

	using vector_operator::operator();


	//void calibrate()
	//{
	//	// calibrate nodes 
	//	int settleTime = _local_srate * 2;
	//	_dummyNode.setFrequency(CALIBRATION_CYCLES, _local_srate);

	//	while (settleTime-- > 0) {
	//		_dummyNode.doCalcStep(true, true);
	//		node.doCalcStep(true, true);
	//	}
	//	_freqComp = _dummyNode.calcFreqCompensation(CALIBRATION_CYCLES, _local_srate);
	//	node.setFreqCompensation(_freqComp);
	//	node.setFrequency(_freq, _local_srate);
	//}




	void doCalibration()
	{
		_initialized = false;
		// calibrate nodes 
		int settleTime = _local_srate * 2;
		_dummyNode.setFrequency(CALIBRATION_CYCLES, _local_srate);
		_dummyNode.reset();
		while (settleTime-- > 0) {
			_dummyNode.doCalcStep(true, true);
		}
		_freqComp = _dummyNode.calcFreqCompensation(CALIBRATION_CYCLES, _local_srate);

		_engine_ptr->setFreqCompensation(_freqComp);

		for (int i = 0; i < _nodeCount; i++) {
			_engine_ptr->setNodeFrequency(i, _freqs[i], false);
		}
		_engine_ptr->reset();
	
		_initialized = true;
	}


	void setParams(const atoms& args, bool startup)
	{

		int firstParam = startup ? ARGS_BEFORE_PARAMS : 0;

		if (args.size() > firstParam) {
			_engine_ptr->setParam_t2Overt1(args[firstParam]);
			_dummyNode.set_t2_over_t1(args[firstParam]);
		}
		else if(!_initialized){
			_engine_ptr->setParam_t2Overt1(P_TRATIO);
			_dummyNode.set_t1(1.0);
			_dummyNode.set_t2(P_TRATIO);

		}

		if (args.size() > firstParam + 1) {
			_engine_ptr->setParam_c(args[firstParam + 1]);
			_dummyNode.set_c(args[firstParam + 1]);
		}
		else if (!_initialized) {
			_engine_ptr->setParam_c(P_C);
			_dummyNode.set_c(P_C);
		}

		if (args.size() > firstParam + 2) {
			_engine_ptr->setParam_b(args[firstParam + 2]);
			_dummyNode.set_b(args[firstParam + 2]);
		}
		else if (!_initialized) {
			_engine_ptr->setParam_b(P_B);
			_dummyNode.set_b(P_B);
		}

		if (args.size() > firstParam + 3) {
			_engine_ptr->setParam_g(args[firstParam + 3]);
			_dummyNode.set_g(args[firstParam + 3]);
		}
		else if (!_initialized) {
			_engine_ptr->setParam_g(P_G);
			_dummyNode.set_g(P_G);
		}

	}


	void setFreq(int node, float freq) {
		if (node == 0 && !fEqual(freq, _freqs[0])) {
			_freqs[0] = freq;
			_freqs[0] = _freqs[0] < MIN_NODE_FREQ ? MIN_NODE_FREQ : _freqs[0];
			_engine_ptr->setNodeFrequency(node, _freqs[0], true);
		}
		else if (!fEqual(freq, _freqs[node])) {
			_freqs[node] = freq;
			float thisF = _freqs[node] * _freqs[0];
			thisF = thisF < MIN_NODE_FREQ ? MIN_NODE_FREQ : thisF;
			_engine_ptr->setNodeFrequency(node, thisF, false);
		}
	}


	bool fEqual(float f1, float f2) {
		return f2 > f1 - FLOAT_EPSILON && f2 < f1 + FLOAT_EPSILON;
	}


	void tester(int nodes, int samples) {

		std::shared_ptr<MatsuokaEngine> _testPtr = std::shared_ptr<MatsuokaEngine>(
			new  MatsuokaEngine(_local_srate, _send_noteTriggers, false, false));
		_dummyNode = MatsuNode();


		// set up nodes and ins/outs for them
		for (int nodeID = 0; nodeID < nodes; ++nodeID) {
			_testPtr->setNodeQuantiser_Grid(nodeID, MatsuokaEngine::gridType::unQuantised);
			_ins.push_back(std::make_unique<inlet<>>(this, "(signal) freq input " + std::to_string(nodeID)));
			_outs.push_back(std::make_unique<outlet<>>(this, "(signal) signal output " + std::to_string(nodeID), "signal"));
			_trigs[nodeID].setLength((int)(TRIGGER_WIDTH * _local_srate));

			if (nodeID != 0) {
				_testPtr->addChild(0, nodeID);
			}
		}


		vector<float> curvX = { DEFAULT_CURVE_X };
		vector<float> curvY = { DEFAULT_CURVE_Y };

		_testPtr->loadConnectionWeightCurve(curvX, curvY);
		_testPtr->setUnityConnectionWeight(UNITY_CONN_WEIGHT);
		_testPtr->setConnectionWeightScaling(true);

		// set frequencies and connect all nodes to all others, with 0.1 signal weight
		for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) {
			//set all frequencies to 1
			_testPtr->setNodeFrequency(nodeID, 1.0, false);
			for (int connectToID = 0; connectToID < _nodeCount; ++connectToID) {
				if (nodeID != connectToID) {
					_testPtr->setConnection(nodeID, connectToID, 0.1);
				}
			}
		}

		_testPtr->doQueuedActions();
		calibrate.set();

		_initialized = true;

		// For each frame in the vector calc each channel
		for (auto frame = 0; frame< samples; ++frame) {



			_testPtr->step();


			// signals
			for (int channel = 0; channel < _nodeCount; ++channel) {
				// send to max output

				_outRingBuff[channel].pushSample((float)_testPtr->getNodeOutput(channel, 0, _send_noteTriggers));
				_outRingBuff[channel].getDelayed(0);
			}
			if (_send_noteTriggers) {
				auto noteEvents = _testPtr->getEvents();
				//for each (auto note in noteEvents) { // AppleClang doesn't like for each here. Can't understand why, switched to trad index loop
				for (auto i = 0; i< noteEvents.size(); i++) {
					_trigs[noteEvents[i].nodeID].setTrigger();
				}
				for (int channel = 0; channel < _nodeCount; channel++) {
					_trigs[channel].tick();
				}
			}
		}

	}

};

MIN_EXTERNAL(cpg_net);