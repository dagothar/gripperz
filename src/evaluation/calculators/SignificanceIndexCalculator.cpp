/* 
 * File:   SignificanceIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on November 12, 2015, 5:11 PM
 */

#include "SignificanceIndexCalculator.hpp"
#include <grasps/filters/GraspStatusFilter.hpp>
#include <rwlibs/task/GraspResult.hpp>
#include <grasps/GraspStatistics.hpp>

using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;
using namespace gripperz::models;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::task;
using namespace std;

SignificanceIndexCalculator::SignificanceIndexCalculator() {
}

SignificanceIndexCalculator::~SignificanceIndexCalculator() {
}

QualityIndexValue SignificanceIndexCalculator::calculate(Gripper::Ptr gripper, Grasps grasps) {
    vector<pair<class GraspSubTask*, class GraspTarget*> > all_targets = grasps->getAllTargets();

    int all_tasks = all_targets.size();
    int samples = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Skip);
    all_tasks -= samples;
    
    int successes = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Success);
    int interferences = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Interference);
    
    double valid_tasks = successes + interferences;
    double significance = 1.0 * valid_tasks / all_tasks;
    
    return significance;
}
