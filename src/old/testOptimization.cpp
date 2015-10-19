/**
 * @file testMath.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "OptimizationTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <math.hpp>


using namespace std;
using namespace gripperz::math;


BOOST_AUTO_TEST_CASE (ParameterMappingTest) {
	ParameterMapping::Range before{-10.0, 10.0};
	ParameterMapping::Range after{0.0, 1.0};
	
	ParameterMapping::Map map(3, {before, after});
	
	ParameterMapping mapping(map);
	
	/* test packing */
	vector<double> p0{-10.0, 0.0, 10.0};
	vector<double> p1 = mapping.map(p0);
	BOOST_CHECK (p1[0] == 0.0);
	BOOST_CHECK (p1[1] == 0.5);
	BOOST_CHECK (p1[2] == 1.0);
	
	/* test unpacking */
	vector<double> p2 = mapping.unmap(p1);
	BOOST_CHECK (p2[0] == -10.0);
	BOOST_CHECK (p2[1] == 0.0);
	BOOST_CHECK (p2[2] == 10.0);
}


BOOST_AUTO_TEST_CASE (ObjectiveFunctionsTest) {
	class TestFunction : public MultiObjectiveFunction {
	public:
		virtual vector<double> evaluate(const vector<double>& x) {
			vector<double> res{x[0] + x[1], x[0] * x[1]};
			return res;
		}
	} func;
	
	/* test multi obj function */
	vector<double> x0{1, 2};
	vector<double> e0{3, 2};
	vector<double> y0 = func.evaluate(x0);
	BOOST_CHECK_EQUAL_COLLECTIONS (y0.begin(), y0.end(), e0.begin(), e0.end());
	
	/* test function combining */
	vector<double> weights{1, 1};
	CombineObjectives::Ptr method = CombineObjectivesFactory::make("sum", weights);
	ObjectiveFunction::Ptr comb = new CombinedFunction(&func, method);
	double y1 = comb->evaluate(x0);
	BOOST_CHECK (y1 == 2.5);
	
	/* test function reverting */
	ObjectiveFunction::Ptr rev = new RevertedFunction(comb);
	double y2 = rev->evaluate(x0);
	BOOST_CHECK (y2 == -2.5);
	
	/* test function mapping */
	ParameterMapping::Range before{0.0, 2.0};
	ParameterMapping::Range after{0.0, 1.0};
	ParameterMapping::Map map(2, {before, after});
	ParameterMapping mapping(map);
	ObjectiveFunction::Ptr mapped = new MappedFunction(comb, &mapping);
	double y3 = mapped->evaluate(x0);
	BOOST_CHECK (y3 == 1);
	//cout << y3;
}
