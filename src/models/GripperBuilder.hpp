/**
 * @file GripperBuilder.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <math/Types.hpp>
#include "OldGripper.hpp"

namespace gripperz {
namespace models {

/**
 * @class GripperBuilder
 * @brief Interface for gripper building.
 * Construct a gripper model from a vector of parameters.
 */
class GripperBuilder {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperBuilder> Ptr;

public:
	/**
	 * @brief Construct a gripper from a vector of parameters.
	 */
	virtual OldGripper::Ptr vectorToGripper(const math::Vector& params) = 0;
	
	/**
	 * @brief Convert a gripper into a vector of parameters.
	 */
	virtual math::Vector gripperToVector(OldGripper::Ptr gripper) = 0;
};

} /* models */
} /* gripperz */
