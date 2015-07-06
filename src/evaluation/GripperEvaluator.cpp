/**
 * @file GripperEvaluator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "GripperEvaluator.hpp"

#include <algorithm>
#include <rwlibs/task/GraspTask.hpp>
#include <rwlibs/task/GraspTarget.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;


GripperEvaluator::GripperEvaluator(TaskDescription::Ptr context) :
	_context(context)
{}


GripperEvaluator::~GripperEvaluator() 
{}


GripperQuality::Ptr GripperEvaluator::evaluateGripper(Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	GripperQuality::Ptr quality = ownedPtr(new GripperQuality());
	
	quality->success = calculateSuccess(gripper, tasks);
	quality->robustness = calculateRobustness(gripper, tasks);
	quality->coverage = calculateCoverage(gripper, tasks);
	quality->alignment = calculateAlignment(gripper, tasks);
	quality->wrench = calculateWrench(gripper, tasks);
	quality->maxstress = calculateStress(gripper, tasks);
	quality->volume = calculateVolume(gripper, tasks);
	
	return quality;
}


double GripperEvaluator::calculateSuccess(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	return 0.0;
}


double GripperEvaluator::calculateRobustness(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	return 0.0;
}


double GripperEvaluator::calculateCoverage(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	return 0.0;
}


double GripperEvaluator::calculateAlignment(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	return 0.0;
}


bool sortf(double a, double b) {
	return (a > b);
}


double GripperEvaluator::calculateWrench(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	DEBUG << "CALCULATING WRENCH - " << endl;
	
	vector<double> wrenches; // used to find the top 10%
	Q wrench(3, 0, 0, 0);

	int successes = 0;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::ObjectSlipped
		) {
			successes++;

			Q result = p.second->getResult()->qualityAfterLifting;

			wrench(0) += result(0);
			wrench(1) += result(1);

			wrenches.push_back(result(1));
		}
	}

	// find top 20%
	sort(wrenches.begin(), wrenches.end(), sortf);

	int num = 0.2 * successes < 1 ? 1 : 0.2 * successes;

	if (wrenches.size() > 0) {
		for (int i = 0; i < num; ++i) {
			wrench(2) += wrenches[i];
		}
	}

	// calculate averages
	if (successes == 0) {
		successes = 1;
	}
	
	wrench(0) /= successes;
	wrench(1) /= successes;
	wrench(2) /= num;

	return wrench(1);
}


double GripperEvaluator::calculateStress(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	return 0.0;
}


double GripperEvaluator::calculateVolume(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
	return 0.0;
}
