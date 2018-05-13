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


#define ARGS_BEFORE_PARAMS 3
#define CALIBRATION_CYCLES 20
#define TRIGGER_WIDTH 0.02

#define FREQ_MIN 0.001f
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


using namespace c74::min;

class cpg_net : public object<cpg_net>, public vector_operator<>
{
private:
	class _ramp {
	public:
		_ramp() {
			setLength(TRIGGER_WIDTH * 44100);
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

	std::shared_ptr<MatsuokaEngine> _engine_ptr;
	MatsuNode _dummyNode;
	double _freqComp{ P_COMPENSATION };
	int _local_srate;
	bool _send_noteTriggers{ true };

	// holds raw output values for interpolation. Barebones ringbuffer approach.
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


	MIN_DESCRIPTION{ "A basic, no frills Matsuoka Oscillator node" };
	MIN_TAGS{ "audio, oscillator" };
	MIN_AUTHOR{ "Cycling '74" };
	MIN_RELATED{ "phasor~" };


	// Because our object defines a constructor (below) these argument definitions are for
	// documentation purposes only.
	argument<int> node_count_arg{ this, "node_count", "The number of nodes in the network." };
	argument<int> sample_rate_arg{ this, "sample_rate", "The sample rate of network calculations." };
	argument<int> trigger_outputs_arg{ this, "trigger_outputs", "1(default) object has signal and note trigger outputs, 0: object has only signal outputs" };

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



		_engine_ptr = std::shared_ptr<MatsuokaEngine>(
			new  MatsuokaEngine(_local_srate, _send_noteTriggers, false, false));
		_dummyNode = MatsuNode();

		// set up nodes and ins/outs for them
		for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) {
			_engine_ptr->setNodeQuantiser_Grid(nodeID, MatsuokaEngine::gridType::unQuantised);
			_ins.push_back(std::make_unique<inlet<>>(this, "(signal) freq input " + nodeID));
			_outs.push_back(std::make_unique<outlet<>>(this, "(signal) signal output " + nodeID, "signal"));
			_trigs[nodeID].setLength(TRIGGER_WIDTH * _local_srate);

			if (nodeID != 0) {
				_engine_ptr->addChild(0, nodeID);
			}
		}

		if (_send_noteTriggers) {
			for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) 
				_outs.push_back(std::make_unique<outlet<>>(this, "(signal) trigger output " + nodeID, "signal"));
		}


		vector<float> curvX = { DEFAULT_CURVE_X };
		vector<float> curvY = { DEFAULT_CURVE_Y };

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

		cout << "initialised network" << endl;

		_initialized = true;
	}



	//argument<number> a_t1 { this, "t1", "equation parameter t1",
	//	MIN_ARGUMENT_FUNCTION {
	//		node.set_t1( arg);
	//	}
	//};


	message<> bang{ this, "bang", "reset the node",
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

	message<> params{ this, "params",
		MIN_FUNCTION{
		setParams(args, false);
		calibrate.set();
	return {};
	}
	};


	message<> weight{ this, "weight",
		MIN_FUNCTION{
		if (args.size() >= 3) {
			_engine_ptr->setConnection((int)args[0],(int)args[1], (double)args[2]);
		}
	return {};
	}
	};


	message<> scalingCurve{ this, "scalingCurve",
		MIN_FUNCTION{
		if (args.size() % 2 == 0) {
			int itemCount = (int)(args.size() / 2);
			std::vector<float> x, y;
			for (int i = 0; i < itemCount; i++) {
				x.push_back(args[i * 2]);
				y.push_back(args[(i * 2)+1]);
			}
			_engine_ptr->loadConnectionWeightCurve(x, y);
		}
	return {};
	}
	};



	message<> offset_conn{ this, "offset_conn",
		MIN_FUNCTION{
		if (args.size() >= 3) {
			_engine_ptr->setConnectionPhaseOffset((int)args[0],(int)args[1], (double)args[2]);
		}
	return {};
	}
	};


	message<> offset_out{ this, "offset_out",
		MIN_FUNCTION{
		if (args.size() >= 2) {
			_engine_ptr->setNodePhaseOffset((int)args[0],(double)args[1]);
		}
	return {};
	}
	};

	message<> noise{ this, "noise",
		MIN_FUNCTION{
		if (args.size() >= 3) {
			_engine_ptr->setNodeSelfNoise((int)args[0], (double)args[1]);
		}
	return {};
	}
	};


	message<> quant_grid{ this, "quant_grid",
		MIN_FUNCTION{
		using gridType = QuantisedEventQueue::gridType;
		if (args.size() >= 2) {
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

	message<> quant_mult{ this, "quant_mult",
		MIN_FUNCTION{
			if (args.size() >= 2) {
				_engine_ptr->setNodeQuantiser_Multiple((int)args[0], (float)args[1]);
			}
	return {};
	}
	};

	message<> quant_offset{ this, "quant_offset",
		MIN_FUNCTION{
		if (args.size() >= 2) {
			_engine_ptr->setNodeQuantiser_Offset((int)args[0], (float)args[1]);
		}
	return {};
	}
	};

	message<> quant_amount{ this, "quant_amount",
		MIN_FUNCTION{
		if (args.size() >= 1) {
			_engine_ptr->setQuantiseAmount((float)args[0]);
		}
	return {};
	}
	};




	queue calibrate{ this,
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
		// For each frame in the vector calc each channel
		for (auto frame = 0; frame<input.frame_count(); ++frame) {
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
		// For each frame in the vector calc each channel
		for (auto frame = 0; frame<input.frame_count(); ++frame) {

			// if _phase wraps, calc sample from network
			_phase += _phaseStep;
			if (_phase > 1.0) {
				_phase -= 1.0;
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

				output.samples(channel + _nodeCount)[frame] = _trigs[channel].phase() < 1 ? 1 : 0;
				
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
		if (node == 0 && !fEqual(freq,_freqs[0]) ){
			_freqs[0] = freq;
			_freqs[0] = _freqs[0] < FREQ_MIN ? FREQ_MIN : _freqs[0];
			_engine_ptr->setNodeFrequency(node, _freqs[0], true);
		}
		else if(!fEqual(freq, _freqs[node])) {
			_freqs[node] = freq;
			float thisF = _freqs[node] * _freqs[0];
			thisF = thisF < FREQ_MIN ? FREQ_MIN : thisF;
			_engine_ptr->setNodeFrequency(node, thisF, false);
		}
	}

	bool fEqual(float f1, float f2) {
		return f2 > f1 - FLOAT_EPSILON && f2 < f1 + FLOAT_EPSILON;
	}


};

MIN_EXTERNAL(cpg_net);