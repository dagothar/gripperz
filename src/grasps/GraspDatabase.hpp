/**
 * @file GraspDatabase.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-07
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <rwlibs/task/GraspTask.hpp>

namespace gripperz {
namespace grasps {

/**
 * @class GraspDatabase
 * @brief A source of grasps for simulating gripper performance.
 */
class GraspDatabase {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GraspDatabase> Ptr;

public:
	/**
	 * @brief Returns a set of tasks.
	 * @param nTargets [in] number of grasp targets
	 */
	virtual rwlibs::task::GraspTask::Ptr getTasks(unsigned nTargets) = 0;
	
	//! Temporary.
	virtual rwlibs::task::GraspTask::Ptr getSamples() = 0;
};

} /* grasps */
} /* gripperz */
