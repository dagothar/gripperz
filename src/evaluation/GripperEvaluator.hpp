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
	virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);

protected:
	//! Calculates the success index of the gripper.
	virtual double calculateSuccess(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the robustness index of the gripper.
	virtual double calculateRobustness(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the coverage index of the gripper.
	virtual double calculateCoverage(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the alignment index of the gripper.
	virtual double calculateAlignment(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the wrench index of the gripper.
	virtual double calculateWrench(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the stress index of the gripper.
	virtual double calculateStress(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the volume index of the gripper.
	virtual double calculateVolume(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);

private:
	context::TaskDescription::Ptr _context;
};

} /* evaluation */
} /* gripperz */
