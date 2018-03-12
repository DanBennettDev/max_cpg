/// @file	
///	@ingroup 	CPG
/// @author		Dan Bennett
///	@license	Usage of this file and its contents is governed by the MIT License


/*
TODO:
	
	Message input to change equation values
*/

#include "matsuNode.h"
#include "c74_min.h"

#define CALIBRATION_CYCLES 20
#define FREQ_MIN 0.001

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
	number _freq{ 50 };

	sample matsuOut_1{ 0 };
	sample matsuOut_2{ 0 };
	sample matsuOut_3{ 0 };
	sample matsuOut_4{ 0 };

	lib::interpolator::hermite<sample> interp_herm;
	lib::interpolator::linear<sample> interp_lin;
	bool m_initialized{ false };

public:
	

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
			node.set_t2_over_t1(args[1]);
			dummyNode.set_t2_over_t1(args[1]);
		}
		if (args.size() > 2) {
			node.set_c(args[2]);
			dummyNode.set_c(args[2]);
		}

		if (args.size() > 3) {
			node.set_b(args[3]);
			dummyNode.set_b(args[3]);
			cout << "B: " << (float)args[3] << endl;

		}
		if (args.size() > 4) {
			node.set_g(args[4]);
			dummyNode.set_g(args[4]);
			cout << "G: " << (float)args[4] << endl;

		}

		calibrate();

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
		freq = freq < FREQ_MIN ? freq = FREQ_MIN : freq;
		_freq = freq;
		if (m_initialized) {
			if (local_srate == (int)samplerate()) {
				node.setExternalInput(in);
				node.setFreqCompensation(freqComp);
				node.setFrequency(freq, local_srate);

				node.doCalcStep(true, true);
				return node.getOutput();


			} else {
				phase += phaseStep;
				if (phase > 1.0) {
					phase -= 1.0;
					node.setExternalInput(in);
					node.setFreqCompensation(freqComp);
					node.setFrequency(freq, local_srate);

					node.doCalcStep(true, true);
					matsuOut_1 = matsuOut_2;
					matsuOut_2 = matsuOut_3;
					matsuOut_3 = matsuOut_4;
					matsuOut_4 = node.getOutput();
				} else if (phase < 0.0) { // sholdn't happen
					phase += 1.0;
				}

				if (local_srate > 11024) {
					return interp_herm(matsuOut_1, matsuOut_2, matsuOut_3, matsuOut_4, phase);
				} else {
					return interp_lin(matsuOut_1, matsuOut_2, matsuOut_3, matsuOut_4, phase);
				}
			}
		}
		return 0;


	}

	void calibrate()
	{
		// calibrate nodes 
		int settleTime = local_srate * 2;
		dummyNode.setFrequency(CALIBRATION_CYCLES, local_srate);

		while (settleTime-- > 0) {
			dummyNode.doCalcStep(true, true);
			node.doCalcStep(true, true);
		}
		freqComp = dummyNode.calcFreqCompensation(CALIBRATION_CYCLES, local_srate);
		node.setFreqCompensation(freqComp);
		node.setFrequency(_freq, local_srate);
	}

};

MIN_EXTERNAL(node_freq);
