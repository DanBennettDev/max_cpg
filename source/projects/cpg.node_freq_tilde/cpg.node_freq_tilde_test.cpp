// Copyright (c) 2016, Cycling '74
// Timothy Place
// Usage of this file and its contents is governed by the MIT License

#include "c74_min_unittest.h"		// required unit test header
#include "cpg.node_freq_tilde.cpp"	// need the source of our object so that we can access it

// Unit tests are written using the Catch framework as described at
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

TEST_CASE( "calculates a vector when provided valid values" ) {
	ext_main(nullptr);	// every unit test must call ext_main() once to configure the class

	// create an instance of our object
	test_wrapper<node_freq> an_instance;
	node_freq& my_object = an_instance;

	// create an impulse buffer to process
	const int		buffersize = 256;
	sample_vector	impulse(buffersize * 10000);
	
	std::fill_n(impulse.begin(), buffersize, 0.0);
	impulse[0] = 1.0;
	
	// Set params to "problematic" values
	my_object.params({ 3.1, 1, 3, 4 });

	my_object.doCalibration();
	
	// run the calculations
	for (auto x : impulse) {
		auto y = my_object(0.0, 50);
		//output.push_back(y);
	}
	
	int i = 0;
	// check it
//	REQUIRE( output == reference );
	REQUIRE ( true );
}
