/**
 * @file trying_opt.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#include <iostream>
#include <math.hpp>
#include <optimization.hpp>
#include <util.hpp>


using namespace std;
using namespace gripperz::math;
using namespace gripperz::optimization;
using namespace gripperz::util;


class TestFunction: public ObjectiveFunction {
public:
	typedef rw::common::Ptr<TestFunction> Ptr;
	
	Scalar evaluate(const Vector& args) {
		return args[0] * args[0] + args[1] * args[1];
	}
};


int fev = 0;


void callback(const Vector& args, Scalar result) {
	++fev;
	cout << "f(" << args[0] << ", " << args[1] << ") = " << result << endl;
}


int main(int argc, char* argv[]) {
	/* create objective function */
	ObjectiveFunction::Ptr objective = new Rosenbrock(100, 1);
	objective = new CallbackFunction(objective, callback);
	
	/* create optimizer */
	BOBYQAOptimizer::ConstraintList constr({{0, 5}, {0, 5}});
	BOBYQAOptimizer::Configuration opt_config;
	opt_config.initialTrustRegionRadius = 0.01;
	opt_config.finalTrustRegionRadius = 0.00001;
	Optimizer::Ptr optimizer = new BOBYQAOptimizer(opt_config, constr);
	
	/* optimize */
	Vector initialGuess({0.05, 0.5});
	Vector result = optimizer->minimize(objective, initialGuess);
	
	/* print results */
	cout << "Finished.\n" << "Minimum of " << objective->evaluate(result) << " at (";
	cout << result[0] << ", " << result[1] << ")." << endl;
	cout << "fev = " << fev << endl;
	
	return 0;
}
