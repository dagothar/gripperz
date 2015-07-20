/**
 * @file OptimizerFactory.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#include "OptimizerFactory.hpp"
#include <rw/common/macros.hpp>


using namespace gripperz::optimization;
using namespace rw::common;


Optimizer::Ptr OptimizerFactory::makeOptimizer(const std::string& id, unsigned dim) {
	if (id == "bobyqa") {
		return makeBOBYQAOptimizer(dim);
	}
	
	else if (id == "simplex") {
		return makeSimplexOptimizer();
	}
	
	RW_THROW ("Optimizer with id " << id << "is not available");
	return NULL;
}


BOBYQAOptimizer::Ptr OptimizerFactory::makeBOBYQAOptimizer(unsigned dim, double rho_init, double rho_stop, unsigned max_fev) {
	BOBYQAOptimizer::ConstraintList constr(dim, {0, 1});
	
	BOBYQAOptimizer::Configuration opt_config;
	opt_config.initialTrustRegionRadius = rho_init;
	opt_config.finalTrustRegionRadius = rho_stop;
	opt_config.maxNOfEvaluations = max_fev;
	
	BOBYQAOptimizer::Ptr optimizer = ownedPtr(new BOBYQAOptimizer(opt_config, constr));
	
	return optimizer;
}


SimplexOptimizer::Ptr OptimizerFactory::makeSimplexOptimizer(double rho_init, double rho_stop, unsigned max_fev) {
	SimplexOptimizer::Configuration opt_config;
	opt_config.initialSize = rho_init;
	opt_config.finalSize = rho_stop;
	opt_config.maxNOfEvaluations = max_fev;
	
	SimplexOptimizer::Ptr optimizer = ownedPtr(new SimplexOptimizer(opt_config));
	
	return optimizer;
}


CoordinateDescentOptimizer::Ptr OptimizerFactory::makeCoordinateDescentOptimizer(double rho_init, double rho_stop, unsigned max_fev) {
	CoordinateDescentOptimizer::Configuration config;
	config.kInitialStepSize = rho_init;
	config.kFinalStepSize = rho_stop;
	config.maxNOfEvaluations = max_fev;
	
	CoordinateDescentOptimizer::Ptr optimizer = ownedPtr(new CoordinateDescentOptimizer(config));
	
	return optimizer;
}
