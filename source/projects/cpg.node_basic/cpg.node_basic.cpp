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

using namespace c74::min;

class node_basic : public object<node_basic>, public sample_operator<7,1> {
private:
	number inVal;
	MatsuNode node;
	MatsuNode dummyNode;
	double freq;
	double freqComp;

	int testCounter{ 0 };


public:
	bool m_initialized{ false };

	MIN_DESCRIPTION { "A basic, no frills Matsuoka Oscillator node" };
	MIN_TAGS		{	"audio, oscillator"		};
	MIN_AUTHOR		{	"Cycling '74"			};
	MIN_RELATED		{	"phasor~" };

	node_basic(const atoms& args = {})
	{
		cout << "started" << endl;

		inVal = 0;
		node = MatsuNode();
		dummyNode = MatsuNode();
		cout << "initialised matsunode" << endl;

		node.setExternalInput(inVal);


		if (args.size() > 0) {
			node.set_t1(args[0]);
		}
		if (1 < args.size()) {
			node.set_t2(args[1]);
		}
		if (args.size() > 2) {
			node.set_c1(args[2]);
		}
		if (args.size() > 3) {
			node.set_c2(args[3]);
		}
		if (args.size() > 4) {
			node.set_b(args[4]);
		}
		if (args.size() > 5) {
			node.set_g(args[5]);
		}

		// calibrate nodes
		freq = 1;
		int settleTime = samplerate();
		node.setFrequency(freq, samplerate());
		dummyNode.setFrequency(freq, samplerate());

		while (settleTime-- > 0) {
			dummyNode.doCalcStep(true, true);
			node.doCalcStep(true, true);
		}
		freqComp = dummyNode.calcFreqCompensation(4, samplerate());

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
