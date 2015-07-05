/**
 * @file GripperEvaluator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "GripperEvaluator.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::context;


GripperEvaluator::GripperEvaluator(TaskDescription::Ptr context) :
	_context(context)
{}


GripperEvaluator::~GripperEvaluator() 
{}


GripperQuality::Ptr GripperEvaluator::evaluateGripper(Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
}
