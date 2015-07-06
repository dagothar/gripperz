/**
 * @file GripperBuilder.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Gripper.hpp"

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
	virtual Gripper::Ptr parametersToGripper(const std::vector<double>& params) = 0;
	
	/**
	 * @brief Convert a gripper into a vector of parameters.
	 */
	virtual std::vector<double> gripperToParameters(Gripper::Ptr gripper) = 0;
};

} /* models */
} /* gripperz */
