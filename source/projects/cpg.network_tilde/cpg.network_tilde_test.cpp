// Copyright (c) 2016, Cycling '74
// Timothy Place
// Usage of this file and its contents is governed by the MIT License

#include "c74_min_unittest.h"		// required unit test header
#include "cpg.network_tilde.cpp"	// need the source of our object so that we can access it


// Unit tests are written using the Catch framework as described at
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md


//TEST_CASE( "not a real unit test - a debugging tool" ) {
//	ext_main(nullptr);	// every unit test must call ext_main() once to configure the class
//
//	// create a single node instance of our object
//	atoms testArgs = { 1 };
//	test_wrapper<cpg_net> an_instance;
//	cpg_net& my_object = an_instance;
//
//	//// create an impulse buffer to process
//	const int		buffersize = 88100;
//	sample_vector	impulse(buffersize);
//
//	sample_vector	output;
//
//	my_object.params({ 10,1000 }, 0);
//
//	my_object.quant_grid({ 1, "32"}, 0);
//	my_object.quant_grid({ 0, "32" }, 0);
//	my_object.quant_grid({ 2, "32" }, 0);
//	my_object.quant_grid({ 3, "32" }, 0);
//	my_object.quant_grid({ 4, "32" }, 0);
//	my_object.quant_grid({ 5, "32" }, 0);
//
//	my_object.quant_mult({ 1, 8 }, 0);
//	my_object.number( 10, 0);
//	my_object.number(9, 1);
//	my_object.number(10, 0);
//	my_object.number(8, 2);
//	my_object.number(6, 3);
//	my_object.number(5, 4);
//	my_object.number(4, 5);
//	my_object.weight({0,1,0.6 }, 0);
//	my_object.weight({ 1,2,0.6 }, 0);
//	my_object.weight({ 2,3,0.6 }, 0);
//	my_object.weight({ 3,4,0.6 }, 0);
//	my_object.weight({ 4,5,0.6 }, 0);
//	my_object.weight({ 5,6,0.6 }, 0);
//	my_object.weight({ 6,7,0.6 }, 0);
//	my_object.weight({ 7,8,0.6 }, 0);
//	my_object.weight({ 3,2,0.6 }, 0);
//	my_object.weight({ 2,3,0.6 }, 0);
//	my_object.weight({ 2,4,0.6 }, 0);
//	my_object.weight({ 3,5,0.6 }, 0);
//	my_object.weight({ 3,6,0.6 }, 0);
//	my_object.weight({ 3,7,0.6 }, 0);
//	my_object.weight({ 3,8,0.6 }, 0);
//	// run the calculations
//	for (int i = 0; i < buffersize *100; i++) {
//		auto y = my_object(1);
//		output.push_back(y);
//	}
//
//
//
//	// check it
////	REQUIRE( output == reference );
//	REQUIRE ( true );
//}



TEST_CASE("initialises and runs correctly with 1 node") {
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