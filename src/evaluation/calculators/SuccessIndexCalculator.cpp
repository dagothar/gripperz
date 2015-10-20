/* 
 * File:   SuccessIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:26 AM
 */

#include "SuccessIndexCalculator.hpp"
#include <rwlibs/task/GraspTask.hpp>
#include <grasps/GraspStatistics.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace gripperz::models;
using namespace gripperz::grasps;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;
using namespace rwlibs::task;

SuccessIndexCalculator::SuccessIndexCalculator() {
}

SuccessIndexCalculator::~SuccessIndexCalculator() {
}

QualityIndexValue SuccessIndexCalculator::calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {
    DEBUG << "CALCULATING SUCCESS" << endl;
    
    vector<pair<class GraspSubTask*, class GraspTarget*> > allTargets = grasps->getAllTargets();

    int nAllTargets = allTargets.size();
    int successes = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Success);
    int filtered = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Filtered);
    int failures = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::SimulationFailure);
    int samples = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Skip);

    DEBUG << "alltargets= " << nAllTargets << endl;
    DEBUG << "successes= " << successes << endl;
    DEBUG << "filtered= " << filtered << endl;
    DEBUG << "failures= " << failures << endl;
    DEBUG << "samples= " << samples << endl;

    double validTasks = nAllTargets - filtered - failures - samples;

    if (validTasks == 0) {
        RW_WARN("No valid tasks");
        return 0.0;
    }

    double successIndex = 1.0 * successes / validTasks;

    return successIndex;
}
