/// @file	
///	@ingroup 	CPG
/// @author		Dan Bennett
///	@license	Usage of this file and its contents is governed by the MIT License


/*
	TODO:
		Add interpolation options: Cheaper for enveloping uses


*/

#include "matsuNode.h"
#include "c74_min.h"

using namespace c74::min;

class node_basic : public object<node_basic>, public sample_operator<7,1> {
private:
	number inVal;
	MatsuNode node;
	MatsuNode dummyNode;
	int local_srate;

	number phase{ 0 };
	number phaseStep;

	sample matsuOut_1{ 0 };
	sample matsuOut_2{ 0 };
	sample matsuOut_3{ 0 };
	sample matsuOut_4{ 0 };

	lib::interpolator::hermite<sample> interp_herm;
	lib::interpolator::linear<sample> interp_lin;

public:
	bool m_initialized{ false };

	MIN_DESCRIPTION { "A basic, no frills Matsuoka Oscillator node" };
	MIN_TAGS		{	"audio, oscillator"		};
	MIN_AUTHOR		{	"Cycling '74"			};
	MIN_RELATED		{	"phasor~" };

	// Because our object defines a constructor (below) these argument definitions are for
	// documentation purposes only.
	argument<int> sample_rate_arg{ this, "sample_rate", "The sample rate of node calculations" };

	node_basic(const atoms& args = {})
	{
		m_initialized = 0;

		cout << "started" << endl;

		inVal = 0;
		node = MatsuNode();
		dummyNode = MatsuNode();
		cout << "initialised matsunode" << endl;

		node.setExternalInput(inVal);


		if (args.size() > 0) {
			unsigned srate_in = (unsigned)(int)args[0];
			cout << "sample rate requested: " << srate_in << endl;
			if ((int)srate_in > samplerate()/2 || (int)srate_in < 0) {
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

		//if (1 < args.size()) {
		//	node.set_t2(args[1]);
		//}
		//if (args.size() > 2) {
		//	node.set_c1(args[2]);
		//}
		//if (args.size() > 3) {
		//	node.set_c2(args[3]);
		//}
		//if (args.size() > 4) {
		//	node.set_b(args[4]);
		//}
		//if (args.size() > 5) {
		//	node.set_g(args[5]);
		//}

		//// calibrate nodes (not required in basic case)
		//freq = 1;
		//int settleTime = local_srate;
		//node.setFrequency(freq, local_srate);
		//dummyNode.setFrequency(freq, local_srate);

		//while (settleTime-- > 0) {
		//	dummyNode.doCalcStep(true, true);
		//	node.doCalcStep(true, true);
		//}
		//freqComp = dummyNode.calcFreqCompensation(4, local_srate);

		m_initialized = true;
	}

	~node_basic(){
		// object-specific tear-down code here
	}

	inlet<>			in	{ this, "(signal) input"};
	inlet<>			t1	{ this, "(signal) equation param t1" };
	inlet<>			t2	{ this, "(signal) equation param t2"};
	inlet<>			c1	{ this, "(signal) equation param c1" };
	inlet<>			c2	{ this, "(signal) equation param c2"};
	inlet<>			b	{ this, "(signal) equation param b"};
	inlet<>			g	{ this, "(signal) equation param g"};

	outlet<>		out	{ this, "(signal) ramp wave", "signal" };


	//argument<number> a_t1 { this, "t1", "equation parameter t1",
	//	MIN_ARGUMENT_FUNCTION {
	//		node.set_t1( arg);
	//	}
	//};


	message<> bang { this, "bang", "reset the node",
		MIN_FUNCTION {
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


	sample operator()(sample in, sample t1, sample t2, sample c1, sample c2, sample b, sample g) {

		if (m_initialized) {
			if (local_srate == (int)samplerate()) {
				matsuOut_1 = matsuOut_2;
				matsuOut_2 = matsuOut_3;
				matsuOut_3 = matsuOut_4;
				matsuOut_4 = runMatsu(in, t1, t2, c1, c2, b, g);
				return matsuOut_1;

			} else {
				phase += phaseStep;
				if (phase > 1.0) {
					phase -= 1.0;

					matsuOut_1 = matsuOut_2;
					matsuOut_2 = matsuOut_3;
					matsuOut_3 = matsuOut_4;
					matsuOut_4 = runMatsu(in, t1, t2, c1, c2, b, g);
				} else if (phase < 0.0) { // sholdn't happen
					phase += 1.0;
				}
				if (local_srate > 11025) {
					return interp_herm(matsuOut_1, matsuOut_2, matsuOut_3, matsuOut_4, phase);
				} else {
					return interp_lin(matsuOut_1, matsuOut_2, matsuOut_3, matsuOut_4, phase);
				}
			}
		}
		return 0;

	}

	sample runMatsu(sample in, sample t1, sample t2, sample c1, sample c2, sample b, sample g)
	{
		node.set_t1(t1);
		node.set_t2(t2);
		node.set_c1(c1);
		node.set_c2(c2);
		node.set_b(b);
		node.set_g(g);
		node.setExternalInput(in);
		node.doCalcStep(true, true);
		return node.getOutput();
	}



};

MIN_EXTERNAL(node_basic);
