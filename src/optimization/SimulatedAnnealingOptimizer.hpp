/**
 * @file SimulatedAnnealingOptimizer.hpp
 * @author Adam Wolniakowski
 * @date 2015-09-06
 */

#pragma once

#include "Optimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class CLASS
 * @brief
 */
class SimulatedAnnealingOptimizer: public Optimizer {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<SimulatedAnnealingOptimizer> Ptr;
	
	//! Configuration.
	struct Configuration {
		double rho_0;
		double rho_final;
		unsigned maxNOfEvaluations;
		
		Configuration() :
			rho_0(0.1),
			rho_final(1e-3),
			maxNOfEvaluations(1000)
		{}
	};

public:
	SimulatedAnnealingOptimizer(const Configuration& config=Configuration());
	
	virtual ~SimulatedAnnealingOptimizer();
	
	/**
	 * @brief Minimizes the objective function starting from the initial guess using simulated annealing method.
	 */
	virtual math::Vector minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);

protected:
	/**
	 * @brief Calculates new state acceptance probability based on energies e0 and e1 and temperature.
	 */
	virtual double calculateAcceptanceProbability(double e0, double e1, double T);
	
private:
	Configuration _config;
};

} /* optimization */
} /* gripperz */
