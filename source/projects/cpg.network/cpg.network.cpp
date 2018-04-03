/// @file	
///	@ingroup 	CPG
/// @author		Dan Bennett
///	@license	Usage of this file and its contents is governed by the MIT License


/*
	No Quantiser on this one

	TODO:

		- float frequencies

		- does phase offset control work?
		
		- add param controls for 
			- delay out change

		- trigger outputs
		- Quantiser stuff

		- OSC front end


	LONGER TERM:
		External input(s)
		Smoothing control changes
		Waveshaping between connections
		
		Do I need a proper shutdown proces?

		PERFORMANCE:
			switchable triggering
			Curve lookup every sample? (for freq check)
			Look for other bottlenecks


*/



#include "c74_min.h"
#include "matsuokaEngine.h"
#include "matsuNode.h"

#define CALIBRATION_CYCLES 20
#define FREQ_MIN 0.001
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



using namespace c74::min;

class cpg_net : public object<cpg_net>, public vector_operator<>
{
private:
	std::shared_ptr<MatsuokaEngine> _engine_ptr;
	MatsuNode _dummyNode;
	double _freqComp{ P_COMPENSATION };
	int _local_srate;

	// holds raw output values for interpolation. Barebones ringbuffer approach.
	DelayLine<float> _outRingBuff[MAX_NODES];
	int _ringIndex{ 0 };
	number _freqs[MAX_NODES]{ 0 };
	number _phase{ 0 };
	number _phaseStep;

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

		// TODO: TEST ONLY - REMOVE 

		_engine_ptr = std::shared_ptr<MatsuokaEngine>(
			new  MatsuokaEngine(_local_srate, true, false, true));
		_dummyNode = MatsuNode();

		// set up nodes and ins/outs for them
		for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) {
			_ins.push_back(std::make_unique<inlet<>>(this, "(signal) audio input"));
			_outs.push_back(std::make_unique<outlet<>>(this, "(signal) audio output", "signal"));

			if (nodeID != 0) {
				_engine_ptr->addChild(0, nodeID);
			}
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
				_engine_ptr->setNodeFrequency(inlet,args[0], false);
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
			int itemCount = args.size() / 2;
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







	message<> offset{ this, "offset",
		MIN_FUNCTION{
		if (args.size() >= 3) {
			_engine_ptr->setConnectionPhaseOffset((int)args[0],(int)args[1], (double)args[2]);
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
		sample freq;
		for (auto frame = 0; frame<input.frame_count(); ++frame) {
			_engine_ptr->step();
			
			for (int channel = 0; channel < output.channel_count(); ++channel) {
				// send to max output
				if (_ins[channel]->has_signal_connection()){
					freq = input.samples(channel)[frame];
					freq = freq < FREQ_MIN ? freq = FREQ_MIN : freq;
					_engine_ptr->setNodeFrequency(channel, freq ,false);
				}
				_outRingBuff[channel].pushSample(_engine_ptr->getNodeOutput(channel));
				output.samples(channel)[frame] = _outRingBuff[channel].getDelayed(0);
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
			}
			else if (_phase < 0.0) { // sholdn't happen
				_phase += 1.0;
			}

			// regardless of _phase wrap, calculate our interpolated sample for outputs
			for (int channel = 0; channel < output.channel_count(); ++channel) {
				sample freq;
				if (_ins[channel]->has_signal_connection()) {
					freq = input.samples(channel)[frame];
					freq = freq < FREQ_MIN ? freq = FREQ_MIN : freq;
					_engine_ptr->setNodeFrequency(channel, freq, false);
				}
				_outRingBuff[channel].pushSample(_engine_ptr->getNodeOutput(channel));

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

		int firstParam = startup ? 2 : 0;

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


};

MIN_EXTERNAL(cpg_net);