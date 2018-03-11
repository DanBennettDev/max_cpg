/// @file	
///	@ingroup 	minexamples
///	@copyright	Copyright (c) 2017, Cycling '74
/// @author		Timothy Place
///	@license	Usage of this file and its contents is governed by the MIT License


/*
TODO:
1) Simple version without just direct parameter controls
- add external input and input handler

2) version with freq directly settable
- set parameters via message
- offline freq comp recalc when equation parameters are changed
-



*/

#include "matsuNode.h"
#include "c74_min.h"

#define CALIBRATION_CYCLES 20

using namespace c74::min;

class node_freq : public object<node_freq>, public sample_operator<2, 1>
{
private:
	number inVal;
	MatsuNode node;
	MatsuNode dummyNode;
	double freqComp;
	int local_srate;

	int testCounter{ 0 };

	number phase{ 0 };
	number phaseStep;

	sample matsuOut_1{ 0 };
	sample matsuOut_2{ 0 };
	sample matsuOut_3{ 0 };
	sample matsuOut_4{ 0 };

	lib::interpolator::cubic<sample> interpolate;

public:
	bool m_initialized{ false };

	MIN_DESCRIPTION{ "A basic, no frills Matsuoka Oscillator node" };
	MIN_TAGS{ "audio, oscillator" };
	MIN_AUTHOR{ "Cycling '74" };
	MIN_RELATED{ "phasor~" };

	node_freq(const atoms& args = {})
	{
		m_initialized = false;

		cout << "started" << endl;

		inVal = 0;
		node = MatsuNode();
		dummyNode = MatsuNode();
		cout << "initialised matsunode" << endl;

		node.setExternalInput(inVal);


		if (args.size() > 0) {
			unsigned srate_in = (unsigned)(int)args[0];
			cout << "sample rate requested: " << srate_in << endl;
			if ((int)srate_in > samplerate() / 2 || (int)srate_in < 0) {
				cout << "sample rate must be system sample rate, or else >0 and < system sample-rate / 2: " << srate_in << endl;
				cout << "setting to system sample rate" << endl;

				local_srate = (int)samplerate();
			} else {
				local_srate = srate_in;
			}
		} else {
			local_srate = (int)samplerate();
		}
		phaseStep = local_srate / samplerate();

		node.set_t1(T1_INIT);
		dummyNode.set_t1(T1_INIT);
		node.set_t2(T2_INIT);
		dummyNode.set_t2(T2_INIT);
		node.set_c(C_INIT);
		dummyNode.set_c(C_INIT);
		node.set_b(B_INIT);
		dummyNode.set_b(B_INIT);
		node.set_g(G_INIT);
		dummyNode.set_g(G_INIT);

		if (args.size() > 1) {
			node.set_t1(args[1]);
			dummyNode.set_t1(args[1]);
		}
		if (args.size() > 2) {
			node.set_t2(args[2]);
			dummyNode.set_t2(args[2]);
		}
		if (args.size() > 3) {
			node.set_c1(args[3]);
			dummyNode.set_c1(args[3]);
		}
		if (args.size() > 4) {
			node.set_c2(args[4]);
			dummyNode.set_c2(args[4]);
		}
		if (args.size() > 5) {
			node.set_b(args[5]);
			dummyNode.set_b(args[5]);
		}
		if (args.size() > 6) {
			node.set_g(args[6]);
			dummyNode.set_g(args[6]);
		}

		// calibrate nodes 
		int settleTime = local_srate * 2;
		node.setFrequency(1, local_srate);
		dummyNode.setFrequency(CALIBRATION_CYCLES, local_srate);

		while (settleTime-- > 0) {
			dummyNode.doCalcStep(true, true);
			node.doCalcStep(true, true);
		}
		freqComp = dummyNode.calcFreqCompensation(CALIBRATION_CYCLES, local_srate);
		node.setFreqCompensation(freqComp);
		m_initialized = true;
	}

	~node_freq()
	{
		// object-specific tear-down code here
	}

	inlet<>			in{ this, "(signal) input" };
	inlet<>			freq{ this, "(signal) node frequency" };

	outlet<>		out{ this, "(signal) ramp wave", "signal" };


	//argument<number> a_t1 { this, "t1", "equation parameter t1",
	//	MIN_ARGUMENT_FUNCTION {
	//		node.set_t1( arg);
	//	}
	//};


	message<> bang{ this, "bang", "reset the node",
		MIN_FUNCTION{
		if (m_initialized) {
			node.reset();
		}
	return {};
	}
	};


	//attribute<number> frequency { this, "frequency" , 1.0, description{"Frequency in Hz"},
	//	setter { MIN_FUNCTION {
	//		if (m_initialized) {
	//			m_oscillator.frequency(args[0], samplerate());
	//		}
	//		return args;
	//	}}
	//};


	sample operator()(sample in, sample freq)
	{

		if (m_initialized) {
			if (local_srate == (int)samplerate()) {
				node.setFrequency(freq, local_srate);
				node.doCalcStep(true, true);
				return node.getOutput();


			} else {
				phase += phaseStep;
				if (phase > 1.0) {
					phase -= 1.0;
					node.setFrequency(freq, local_srate);
					node.doCalcStep(true, true);
					matsuOut_1 = matsuOut_2;
					matsuOut_2 = matsuOut_3;
					matsuOut_3 = matsuOut_4;
					matsuOut_4 = node.getOutput();
				} else if (phase < 0.0) { // sholdn't happen
					phase += 1.0;
				}

				return interpolate(matsuOut_1, matsuOut_2, matsuOut_3, matsuOut_4, phase);
			}
		} else {
			return 0;
		}

	}



};

MIN_EXTERNAL(node_freq);
