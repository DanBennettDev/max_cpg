// Copyright (c) 2016, Cycling '74
// Timothy Place
// Usage of this file and its contents is governed by the MIT License

#include "c74_min_unittest.h"		// required unit test header
#include "cpg.network_tilde.cpp"	// need the source of our object so that we can access it


// Unit tests are written using the Catch framework as described at
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md


TEST_CASE("test for observing performance") {
	ext_main(nullptr);	// every unit test must call ext_main() once to configure the class

						// create a single node instance of our object
	atoms testArgs = { 1 };
	test_wrapper<cpg_net> an_instance;
	cpg_net& my_object = an_instance;

	//// create an impulse buffer to process
	const int		buffersize = 88100;
	sample_vector	impulse(buffersize);

	sample_vector	output;

	my_object.tester(10, buffersize * 100);


	// check it
	//	REQUIRE( output == reference );
	REQUIRE(true);
}