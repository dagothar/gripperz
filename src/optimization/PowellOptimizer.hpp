/**
 * @file PowellOptimizer.hpp
 * @author Adam Wolniakowski
 * @date 2015-09-07
 */

#pragma once

#include "Optimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class PowellOptimizer
 * @brief
 */
class PowellOptimizer: public Optimizer {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<PowellOptimizer> Ptr;
	
	//! Configuration.
	struct Configuration {
		double rho_0;
		double rho_final;
		double kSuccessful;
		double kUnsuccesful;
		int maxNOfEvaluations;
		
		Configuration() :
			rho_0(0.1),
			rho_final(1e-3),
			kSuccessful(1.61),
			kUnsuccesful(1.0 / 1.61),
			maxNOfEvaluations(1e3)
		{}
	};

public:
	PowellOptimizer(const Configuration& config=Configuration());
	
	virtual ~PowellOptimizer();
	
	/**
	 * @brief Minimizes the objective function starting from the initial guess.
	 */
	virtual std::vector<double> minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);

private:
	Configuration _config;
};

} /* optimization */
} /* gripperz */
