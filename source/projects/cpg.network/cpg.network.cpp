/// @file	
///	@ingroup 	CPG
/// @author		Dan Bennett
///	@license	Usage of this file and its contents is governed by the MIT License


/*
	No Quantiser on this one

	TODO:

		- add signal inputs for freq


		- add param controls for 
			- delay out change

		Does weight scaling work correctly?


	LONGER TERM:
		External input(s)
		Smoothing control changes
		Waveshaping between connections


*/



#include "c74_min.h"
#include "matsuokaEngine.h"
#include "matsuNode.h"

#define CALIBRATION_CYCLES 20
#define FREQ_MIN 0.001
#define MAX_NODES 16
#define INTERP_SAMPLES 4
#define UNITY_CONN_WEIGHT 1
#define P_TRATIO 4
#define P_C 1
#define P_B 4.07
#define P_G 4.07




using namespace c74::min;

class cpg_net : public object<cpg_net>, public vector_operator<>
{
private:
	std::shared_ptr<MatsuokaEngine> _engine_ptr;
	MatsuNode _dummyNode;
	double _freqComp{ DEFAULTFREQCOMPENSAITON };
	int _local_srate;

	// holds raw output values for interpolation. Barebones ringbuffer approach.
	sample _outRingBuff[MAX_NODES][INTERP_SAMPLES]{ 0 };
	int _ringIndex{ 0 };
	number _freqs[MAX_NODES]{ 0 };
	number _phase{ 0 };
	number _phaseStep;

	lib::interpolator::hermite<sample> _interp_herm;
	lib::interpolator::linear<sample> _interp_lin;
	bool _initialized{ false };
	int _nodeCount{ 1 };

	vector< unique_ptr<inlet<>> >			ins;				///< this object's ins
	vector< unique_ptr<outlet<>> >			outs;				///< this object's outs

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

		_engine_ptr = std::shared_ptr<MatsuokaEngine>(
			new  MatsuokaEngine(_local_srate, true, false, true));
		_dummyNode = MatsuNode();

		// will be adding more (replacing this) later
		ins.push_back(std::make_unique<inlet<>>(this, "(messages) control input"));

		// set up nodes and ins/outs for them
		for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) {
			//ins.push_back(std::make_unique<inlet<>>(this, "(signal) audio input"));
			outs.push_back(std::make_unique<outlet<>>(this, "(signal) audio output " + nodeID, "signal"));
			if (nodeID != 0) {
				_engine_ptr->addChild(0, nodeID);
			}
		}
		// connect all nodes to all others, but with 0 signal weight
		for (int nodeID = 0; nodeID < _nodeCount; ++nodeID) {
			for (int connectToID = 0; connectToID < _nodeCount; ++connectToID) {
				if (nodeID != connectToID) {
					_engine_ptr->setConnection(nodeID, connectToID, 0.0);
				}
			}
		}

		_engine_ptr->setUnityConnectionWeight(UNITY_CONN_WEIGHT);
		_engine_ptr->setConnectionWeightScaling(true);
		_engine_ptr->setParam_t2Overt1(T2_INIT / T1_INIT);
		_engine_ptr->setParam_c(C_INIT);
		_engine_ptr->setParam_b(B_INIT);
		_engine_ptr->setParam_g(G_INIT);
		_engine_ptr->setFreqCompensation(DEFAULTFREQCOMPENSAITON);
		_engine_ptr->setNodeFrequency(0u, 1.0, true);

		_dummyNode.set_t1(T1_INIT);
		_dummyNode.set_t2(T2_INIT);
		_dummyNode.set_c(C_INIT);
		_dummyNode.set_b(B_INIT);
		_dummyNode.set_g(G_INIT);

		setParams(args, true);

		_engine_ptr->calibrate();
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

	message<> offset{ this, "offset",
		MIN_FUNCTION{
		if (args.size() >= 3) {
			_engine_ptr->setConnectionPhaseOffset((int)args[0],(int)args[1], (double)args[2]);
		}
	return {};
	}
	};

	message<> node_freq{ this, "node_freq",
		MIN_FUNCTION{
		if (args.size() >= 2){
			_freqs[(int)args[0]] = args[1];
			_engine_ptr->setNodeFrequency((int)args[0],(int)args[1], false);
		}
	return {};
	}
	};

	message<> net_freq{ this, "net_freq",
		MIN_FUNCTION{
		if (!args.empty()) {
			number freqMult = (float)args[0] / _freqs[0];
			_freqs[0] = args[0];
			_engine_ptr->setNodeFrequency(0,(int)args[0], true);
			for (int i = 1; i < _nodeCount; i++) {
				_freqs[i] *= freqMult;
			}
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
			_engine_ptr->doQueuedActions();
			_engine_ptr->step();
			// send to max output
			for (int channel = 0; channel < _nodeCount; ++channel) {
				output.samples(channel)[frame] = _outRingBuff[channel][_ringIndex];
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

				_ringIndex++;
				if (_ringIndex >= INTERP_SAMPLES) { _ringIndex = 0; }
				_engine_ptr->doQueuedActions();
				_engine_ptr->step();
			}
			else if (_phase < 0.0) { // sholdn't happen
				_phase += 1.0;
			}

			// regardless of _phase wrap, calculate our interpolated sample for outputs
			for (int channel = 0; channel < _nodeCount; ++channel) {
				_outRingBuff[channel][_ringIndex] = _engine_ptr->getNodeOutput(channel);

				if (_local_srate > 11024) {
					output.samples(channel)[frame] =
							_interp_herm(interpRingLookup(_outRingBuff[channel], _ringIndex,-3),
										interpRingLookup(_outRingBuff[channel], _ringIndex, -2),
										interpRingLookup(_outRingBuff[channel], _ringIndex, -1),
										_outRingBuff[channel][_ringIndex],
										_phase);
				}
				else {
					output.samples(channel)[frame] =
						_interp_lin(interpRingLookup(_outRingBuff[channel], _ringIndex, -3),
							interpRingLookup(_outRingBuff[channel], _ringIndex, -2),
							interpRingLookup(_outRingBuff[channel], _ringIndex, -1),
							_outRingBuff[channel][_ringIndex],
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
		if (args.size() > firstParam + 1) {
			_engine_ptr->setParam_c(args[firstParam + 1]);
			_dummyNode.set_c(args[firstParam + 1]);
		}

		if (args.size() > firstParam + 2) {
			_engine_ptr->setParam_b(args[firstParam + 2]);
			_dummyNode.set_b(args[firstParam + 2]);
		}

		if (args.size() > firstParam + 3) {
			_engine_ptr->setParam_g(args[firstParam + 3]);
			_dummyNode.set_g(args[firstParam + 3]);
		}
	}

	// Helper function for our barebones ring buffer approach
	// CAREFUL: offset must be negative - 
	// adding branch here to protect against invalid values would be a waste of resource
	sample interpRingLookup(sample samps[INTERP_SAMPLES], int curr, int offset)
	{
		int raw = curr + offset;
		return samps[raw < 0 ? raw + INTERP_SAMPLES : raw];
	}

};

MIN_EXTERNAL(cpg_net);