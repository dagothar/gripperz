/**
 * @file RobustEvaluationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#include "RobustEvaluationManager.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::models;


RobustEvaluationManager::RobustEvaluationManager(const Configuration& config) :
	GripperEvaluationManager(config)
{
}


RobustEvaluationManager::~RobustEvaluationManager() {
}


GripperQuality::Ptr RobustEvaluationManager::evaluateGripper(Gripper::Ptr gripper) {
}
