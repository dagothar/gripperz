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
class GripperQuality {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperQuality> Ptr;

public:
	GripperQuality();
	
	virtual ~GripperQuality();
	
	friend std::ostream& operator<<(std::ostream& stream, const GripperQuality& q);

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

std::ostream& operator<<(std::ostream& stream, const GripperQuality& q);

} /* models */
} /* gripperz */
