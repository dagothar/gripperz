/**
 * @file testDlibRosenbrock.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include <iostream>
#include <optimization/BOBYQAOptimizer.hpp>
#include <util/Rosenbrock.hpp>


using namespace std;
using namespace gripperz::util;
using namespace gripperz::optimization;


int main(int argc, char* argv[]) {
	/* create objective function */
	ObjectiveFunction::Ptr objFun = new Rosenbrock(1, 100);
	
	/* create optimizer */
	BOBYQAOptimizer::ConstraintList constraints{{0, 2}, {0, 2}};
	BOBYQAOptimizer::Configuration config;
	config.initialTrustRegionRadius = 0.2;

	Optimizer::Ptr optimizer = new BOBYQAOptimizer(config, constraints);
	
	/* perform optimization */
	vector<double> initialGuess{0, 0};
	
	vector<double> result = optimizer->minimize(objFun, initialGuess);
	
	/* print results */
	cout << "x=" << result[0] << " y=" << result[1] << " f(x, y)=" << objFun->evaluate(result) << endl;
	
	return 0;
}
