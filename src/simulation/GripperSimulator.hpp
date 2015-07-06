/**
 * @file GripperSimulator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <rw/kinematics/State.hpp>
#include <rwlibs/task/GraspTask.hpp>

namespace gripperz {
namespace simulation {

/**
 * @class GripperSimulator
 * @brief Interface for gripper simulations.
 */
class GripperSimulator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperSimulator> Ptr;

public:
	/**
	 * @brief Load tasks to simulate.
	 */
	virtual void loadTasks(rwlibs::task::GraspTask::Ptr tasks) = 0;
	
	/**
	 * @brief Get tasks / results.
	 */
	virtual rwlibs::task::GraspTask::Ptr getTasks() = 0;
	
	/**
	 * @brief Starts the simulation.
	 */
	virtual void start(const rw::kinematics::State& initState) = 0;
	
	virtual void stop() = 0;
	
	virtual bool isRunning() = 0;
	
	virtual unsigned getNrTasksDone() const = 0;
};

} /* simulation */
} /* gripperz */
