/**
 * @file GripperOptimizationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Optimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class GripperOptimizationManager
 * @brief Manages optimization of the gripper design.
 */
class GripperOptimizationManager {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperOptimizationManager> Ptr;

public:
	GripperOptimizationManager(Optimizer::Ptr optimizer);
	
	virtual ~GripperOptimizationManager();

private:
	Optimizer::Ptr _optimizer;
};

} /* optimization */
} /* gripperz */
