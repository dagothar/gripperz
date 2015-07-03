/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "OptimizationTest"

#include <boost/test/unit_test.hpp>
#include <optimization.hpp>


using namespace std;
using namespace gripperz::optimization;


BOOST_AUTO_TEST_CASE(ParameterMappingTest) {
	ParameterMapping::Range before{-10.0, 10.0};
	/* test packing */
	
}
