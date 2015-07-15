/**
 * @file OptimizationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#pragma once

#include <rw/common/Ptr.hpp>

namespace gripperz {
namespace math {

/**
 * @class OptimizationManager
 * @brief Manages optimization of the objective function, providing utilities.
 * Utilities include: mapping function parameters into an uniform space, and
 * logging optimization progress.
 */
class OptimizationManager {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<OptimizationManager> Ptr;

public:
	OptimizationManager();
	
	virtual ~OptimizationManager();
};

} /* math */
} /* gripperz */
