/**
 * @file ConstrainedGripperEvaluator.cpp
 * @author Adam Wolniakowski
 * @date 13-07-2015
 */

#include "ConstrainedGripperEvaluator.hpp"

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::context;


ConstrainedGripperEvaluator::ConstrainedGripperEvaluator(TaskDescription::Ptr context, AlignmentCalculator::Ptr alignmentCalculator) :
	GripperEvaluator(context, alignmentCalculator)
{}


ConstrainedGripperEvaluator::~ConstrainedGripperEvaluator() 
{}


bool ConstrainedGripperEvaluator::isSane(models::Gripper::Ptr gripper) {
	if (!GripperEvaluator::isSane(gripper)) return false;
	
	if (calculateStress(gripper) == 0.0) return false;
	
	if (calculateVolume(gripper) == 0.0) return false;
	
	return true;
}
