/**
 * @file SimplexOptimizer.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#pragma once

#include "Optimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class SimplexOptimizer
 * @brief Optimizes using Nelder-Mead method.
 * Implemented by me.
 */
class SimplexOptimizer: public Optimizer {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<SimplexOptimizer> Ptr;
	
	//! Configuration.
	struct Configuration {
		double initialSize;
		double finalSize;
		unsigned maxNOfEvaluations;
		
		Configuration() :
			initialSize(0.1),
			finalSize(1e-3),
			maxNOfEvaluations(1000)
		{}
	};
	
	//! Reflection coefficient.
	static constexpr double kReflection = 1.0;

	//! Expansion coefficient.
	static constexpr double kExpansion = 2.0;

	//! Contraction coefficient.
	static constexpr double kContraction = -0.5;

	//! Reduction coefficient.
	static constexpr double kReduction = 0.5;

public:
	SimplexOptimizer(Configuration config=Configuration());
	
	virtual ~SimplexOptimizer();
	
	/**
	 * @brief Minimizes the objective function starting from the initial guess using Nelder-Mead method.
	 */
	virtual math::Vector minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);

private:
	Configuration _config;
};

} /* optimization */
} /* gripperz */
