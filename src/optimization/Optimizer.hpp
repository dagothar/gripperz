/**
 * @file Optimizer.hpp
 * @author Adam Wolniakowski
 * @date 5-07-2015
 */

#pragma once

#include <math/ObjectiveFunction.hpp>

namespace gripperz {
namespace optimization {

/**
 * @class Optimizer
 * Interface for optimizers.
 */
class Optimizer {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<Optimizer> Ptr;

public:
	/**
	 * @brief Minimizes the objective function starting from the initial guess.
	 */
	virtual math::Vector minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess) = 0;
};

} /* optimization */
} /* gripperz */
