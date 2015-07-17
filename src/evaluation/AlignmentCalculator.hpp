/**
 * @file AlignmentCalculator.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-17
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <rwlibs/task/GraspTask.hpp>

namespace gripperz {
namespace evaluation {

/**
 * @class AlignmentCalculator
 * @brief Interface for calculating alignment.
 */
class AlignmentCalculator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<AlignmentCalculator> Ptr;

public:
	/**
	 * @brief Calculates alignment based on tasks.
	 */
	virtual double calculateAlignment(rwlibs::task::GraspTask::Ptr tasks) = 0;
};

} /* evaluation */
} /* gripperz */
