/**
 * @file GripperEvaluator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <models/Gripper.hpp>
#include <context/TaskDescription.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include "AlignmentCalculator.hpp"
#include "StablePoseAlignment.hpp"

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
	GripperEvaluator(context::TaskDescription::Ptr context, AlignmentCalculator::Ptr alignmentCalculator=rw::common::ownedPtr(new StablePoseAlignment()));
	
	virtual ~GripperEvaluator();
	
	context::TaskDescription::Ptr getContext() { return _context; }
	void setContext(context::TaskDescription::Ptr context) { _context = context; }
	
	AlignmentCalculator::Ptr getAlignmentCalculator() { return _alignmentCalculator; }
	void setAlignmentCalculator(AlignmentCalculator::Ptr calc) { _alignmentCalculator = calc; }
	
	/**
	 * @brief Tests sanity of the gripper design.
	 */
	virtual bool isSane(models::Gripper::Ptr gripper);
	
	/**
	 * @brief Evaluates gripper quality.
	 * Uses gripper parameters and statistics in task performance to calculate the gripper quality.
	 */
	virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples, rwlibs::task::GraspTask::Ptr rtasks=NULL);

protected:
	//! Calculates the success index of the gripper.
	virtual double calculateSuccess(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the robustness index of the gripper.
	virtual double calculateRobustness(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples, rwlibs::task::GraspTask::Ptr rtasks=NULL);
	
	//! Calculates the coverage index of the gripper.
	virtual double calculateCoverage(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples);
	
	//! Calculates the alignment index of the gripper.
	virtual double calculateAlignment(rwlibs::task::GraspTask::Ptr tasks);
	
	//! Calculates the wrench index of the gripper.
	virtual double calculateWrench(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks);
	virtual double calculateTopWrench(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks);
	
	//! Calculates the stress index of the gripper.
	virtual double calculateStress(models::Gripper::Ptr gripper);
	
	//! Calculates the volume index of the gripper.
	virtual double calculateVolume(models::Gripper::Ptr gripper);

private:
	context::TaskDescription::Ptr _context;
	AlignmentCalculator::Ptr _alignmentCalculator;
};

} /* evaluation */
} /* gripperz */
