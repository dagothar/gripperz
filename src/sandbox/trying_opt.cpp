/**
 * @file trying_opt.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#include <iostream>
#include <math.hpp>
#include <optimization.hpp>
#include <util.hpp>
#include <boost/foreach.hpp>


using namespace std;
using namespace gripperz::math;
using namespace gripperz::optimization;
using namespace gripperz::util;


int main(int argc, char* argv[]) {
	/* create objective function */
	ObjectiveFunction::Ptr objective = new Rosenbrock(1, 100);
	
	/* create optimizer */
	//Optimizer::Ptr optimizer = OptimizerFactory::makeBOBYQAOptimizer(2, 0.1, 1e-4);
	Optimizer::Ptr optimizer = OptimizerFactory::makeSimplexOptimizer(0.1, 1e-3);
	OptimizationManager opt_manager(optimizer, {{0, 3}, {0, 3}});
	
	/* optimize */
	Vector initialGuess({0, 1});
	Vector result = opt_manager.optimize(objective, initialGuess, "minimize");
	
	/* print results */
	BOOST_FOREACH (OptimizationManager::LogEntry& entry, opt_manager.getLog()) {
		cout << entry.first << ", " << entry.second << endl;
	}
	
	cout << "Finished.\n" << "Minimum of " << objective->evaluate(result) << " at (";
	cout << result[0] << ", " << result[1] << ").after fev=" << opt_manager.getLog().size() << " evaluations." << endl;
	
	return 0;
}
