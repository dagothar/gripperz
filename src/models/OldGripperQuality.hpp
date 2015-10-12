/**
 * @file GripperQuality.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-13
 */

#pragma once

#include <iostream>
#include <rw/common/Ptr.hpp>

namespace gripperz {
namespace models {

/**
 * @class GripperQuality
 * @brief Stores gripper quality indices.
 */
class OldGripperQuality {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<OldGripperQuality> Ptr;

public:
	OldGripperQuality();
	
	virtual ~OldGripperQuality();
	
	friend std::ostream& operator<<(std::ostream& stream, const OldGripperQuality& q);

	double success;
	double robustness;
	double alignment;
	double coverage;
	double wrench;
	double topwrench;
	double stress;
	double volume;
	double quality;
};

std::ostream& operator<<(std::ostream& stream, const OldGripperQuality& q);

} /* models */
} /* gripperz */
