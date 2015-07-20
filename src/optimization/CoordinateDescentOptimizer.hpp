/**
 * @file CoordinateDescentOptimizer.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-19
 */

#pragma once

#include "Optimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class CoordinateDescentOptimizer
 * @brief Optimizes using coordinate descent method
 */
class CoordinateDescentOptimizer: public virtual Optimizer {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CoordinateDescentOptimizer> Ptr;
	
	//! Configuration.
	struct Configuration {
		double kInitialStepSize;
		double kFinalStepSize;
		double kSuccessful;
		double kUnsuccesful;
		int maxNOfEvaluations;
		
		Configuration() :
			kInitialStepSize(0.1),
			kFinalStepSize(1e-3),
			kSuccessful(1.61),
			kUnsuccesful(1.0 / 1.61),
			maxNOfEvaluations(1e3)
		{}
	};

public:
	CoordinateDescentOptimizer(const Configuration& conf=Configuration());
	
	virtual ~CoordinateDescentOptimizer();
	
	//! @copydoc Optimizer::minimize
	std::vector<double> minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);
	
	Configuration& getConfiguration() { return _conf; }
	void setConfiguration(const Configuration& conf) { _conf = conf; }
	
private:
	Configuration _conf;
};

} /* optimization */
} /* gripperz */
