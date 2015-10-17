/* 
 * File:   CostIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:26 AM
 */

#include "CostIndexCalculator.hpp"
#include <grasps/Types.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace gripperz::models;
using namespace gripperz::grasps;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;
using namespace rwlibs::task;
using namespace rw::math;

CostIndexCalculator::CostIndexCalculator() {
}

CostIndexCalculator::~CostIndexCalculator() {
}

QualityIndexValue CostIndexCalculator::calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps) {

    DEBUG << "CALCULATING WRENCH" << endl;

    double wrenchIndex = 0.0;

    int successes = 0;
    typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget p, grasps->getAllTargets()) {

        if (
                p.second->getResult()->testStatus == GraspResult::Success
                || p.second->getResult()->testStatus == GraspResult::Interference
                ) {
            successes++;

            Q result = p.second->getResult()->qualityAfterLifting;

            wrenchIndex += result(0);
        }
    }

    // calculate averages
    if (successes == 0) {
        return 0.0;
    }

    wrenchIndex /= successes;
   
    return wrenchIndex;
}
