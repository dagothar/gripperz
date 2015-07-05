/**
 * @file GripperEvaluator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <models/Gripper.hpp>
#include <context/TaskDescription.hpp>
#include <rwlibs/task/GraspTask.hpp>

namespace gripperz {
namespace evaluation {
	
/**
 * @class GripperEvaluator
 * @brief Interface for gripper evaluation.
 */
class GripperEvaluator {
public:
	typedef rw::common::Ptr<GripperEvaluator> Ptr;

public:
	/**
	 * @brief Constructor.
	 * @param context [in] task description
	 */
	GripperEvaluator(context::TaskDescription::Ptr context);
	
	virtual ~GripperEvaluator();
	
	context::TaskDescription::Ptr getContext() { return _context; }
	void setContext(context::TaskDescription::Ptr context) { _context = context; }
	
	/**
	 * @brief Evaluates gripper quality.
	 * Uses gripper parameters and statistics in task performance to calculate the gripper quality.
	 */
	virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks);

private:
	context::TaskDescription::Ptr _context;
};

} /* evaluation */
} /* gripperz */
