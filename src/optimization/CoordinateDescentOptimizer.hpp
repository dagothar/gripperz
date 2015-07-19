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

public:
	CoordinateDescentOptimizer();
	
	virtual ~CoordinateDescentOptimizer();
	
	//! @copydoc Optimizer::minimize
	std::vector<double> minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);
};

} /* optimization */
} /* gripperz */
