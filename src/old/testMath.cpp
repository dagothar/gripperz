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
#include <boost/bind.hpp>
#include <math.hpp>


using namespace std;
using namespace gripperz::math;


BOOST_AUTO_TEST_CASE (ParameterMappingTest) {
	Range before{-10.0, 10.0};
	Range after{0.0, 1.0};
	
	ParameterMapping::Map map(3, {before, after});
	
	ParameterMapping mapping(map);
	
	/* test packing */
	vector<double> p0{-10.0, 2.0, 10.0};
	vector<double> p1 = mapping.map(p0);
	BOOST_CHECK (p1[0] == 0.0);
	BOOST_CHECK (p1[1] == 0.6);
	BOOST_CHECK (p1[2] == 1.0);
	
	/* test unpacking */
	vector<double> p2 = mapping.unmap(p1);
	BOOST_CHECK (p2[0] == -10.0);
	BOOST_CHECK (p2[1] == 2.0);
	BOOST_CHECK (p2[2] == 10.0);
}


BOOST_AUTO_TEST_CASE (ObjectiveFunctionsTest) {
	class TestFunction : public MultiObjectiveFunction {
	public:
		virtual Vector evaluate(const Vector& x) {
			Vector res{x[0] + x[1], x[0] * x[1]};
			return res;
		}
	} func;
	
	/* test multi obj function */
	Vector x0{1, 2};
	Vector e0{3, 2};
	Vector y0 = func.evaluate(x0);
	BOOST_CHECK_EQUAL_COLLECTIONS (y0.begin(), y0.end(), e0.begin(), e0.end());
	
	/* test function combining */
	Vector weights{1, 1};
	CombineObjectives::Ptr method = CombineObjectivesFactory::make("sum", weights);
	ObjectiveFunction::Ptr comb = new CombinedFunction(&func, method);
	Scalar y1 = comb->evaluate(x0);
	BOOST_CHECK (y1 == 2.5);
	
	/* test function reverting */
	ObjectiveFunction::Ptr rev = new RevertedFunction(comb);
	Scalar y2 = rev->evaluate(x0);
	BOOST_CHECK (y2 == -2.5);
	
	/* test function mapping */
	Range before{0.0, 2.0};
	Range after{0.0, 1.0};
	ParameterMapping::Map map(2, {before, after});
	ParameterMapping mapping(map);
	ObjectiveFunction::Ptr mapped = new MappedFunction(comb, &mapping);
	Scalar y3 = mapped->evaluate(x0);
	BOOST_CHECK (y3 == 1);
	//cout << y3;
}


BOOST_AUTO_TEST_CASE (ConstrainedFunctionTest) {
	class TestFunction : public ObjectiveFunction {
	public:
		virtual Scalar evaluate(const Vector& x) {
			return x[0] + x[1];
		}
	} func;
	
	RangeList ranges{{0, 1}, {0, 1}};
	Constraint::Ptr constr = new BoxConstraint(ranges);
	ObjectiveFunction::Ptr f = new ConstrainedFunction(&func, constr);
	
	Scalar x1 = f->evaluate({0, 0});
	BOOST_CHECK (x1 == 0.0);
	
	Scalar x2 = f->evaluate({1, 1});
	BOOST_CHECK (x2 == 2.0);
	
	Scalar x3 = f->evaluate({2, 0});
	BOOST_CHECK (x3 == 0.0);
}


BOOST_AUTO_TEST_CASE (CallbackFunctionTest) {
	class TestFunction : public ObjectiveFunction {
	public:
		virtual Scalar evaluate(const Vector& x) {
			return x[0] + x[1];
		}
	} func;
	
	struct Test {
		Test() : args(Vector()), res(0.0) {}
		
		Vector args;
		Scalar res;
		
		void foo(const Vector& a, Scalar r) { args = a; res = r; }
	} test;
	
	boost::function<void(const Vector&, Scalar)> cb = boost::bind(&Test::foo, &test, _1, _2);
	
	ObjectiveFunction::Ptr f = new CallbackFunction(&func, cb);
	f->evaluate({1, 2});
	
	BOOST_CHECK (test.args[0] == 1.0);
	BOOST_CHECK (test.args[1] == 2.0);
	BOOST_CHECK (test.res == 3.0);
}
