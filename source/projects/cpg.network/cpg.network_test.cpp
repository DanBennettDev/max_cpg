// Copyright (c) 2016, Cycling '74
// Timothy Place
// Usage of this file and its contents is governed by the MIT License

#include "c74_min_unittest.h"		// required unit test header
#include "cpg.network.cpp"	// need the source of our object so that we can access it


// Unit tests are written using the Catch framework as described at
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

//
//
//TEST_CASE("wavetable builds correctly") {
//
//	int sampleRate = 44100;
//	CPG _cpg(sampleRate);
//
//	_cpg.setParam_t2Overt1(P_TRATIO);
//	_cpg.setParam_c(P_C);
//	_cpg.setParam_b(P_B);
//	_cpg.setParam_b(P_G);
//	_cpg.setFreqCompensation(P_COMPENSATION);
//	_cpg.setNodeFrequency(0,1,false);
//
//	for (int i = 0; i < 88200; i++) {
//		
//	}
//
//	// calibrate
//
//	// generate wavetable
//
//	// 
//}
//
//
//



TEST_CASE( "initialises and runs correctly with 1 node" ) {
	ext_main(nullptr);	// every unit test must call ext_main() once to configure the class

	// create a single node instance of our object
	atoms testArgs = { 1 };
	test_wrapper<cpg_net> an_instance;
	cpg_net& my_object = an_instance;

	//// create an impulse buffer to process
	const int		buffersize = 88100;
	sample_vector	impulse(buffersize);

	sample_vector	output;

	my_object.params({ 2,1,4.07,4.07}, 0);

	my_object.quant_grid({ 1, "32"}, 0);
	my_object.quant_mult({ 1, 8 }, 0);
	my_object.number( 10, 0);
	my_object.number(9, 1);


	// run the calculations
	for (auto x : impulse) {
		auto y = my_object(x);
		////output.push_back(y);
	}



	// check it
//	REQUIRE( output == reference );
	REQUIRE ( true );
}


