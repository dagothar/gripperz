/* 
 * File:   ClearStatusFilter.cpp
 * Author: dagothar
 * 
 * Created on October 12, 2015, 12:18 PM
 */

#include "ClearStatusFilter.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::task;

ClearStatusFilter::ClearStatusFilter() {
}

ClearStatusFilter::~ClearStatusFilter() {
}

Grasps ClearStatusFilter::filter(Grasps grasps) {
    Grasps filteredGrasps = grasps->clone();

    BOOST_FOREACH(GraspSubTask& subtask, grasps->getSubTasks()) {
        GraspSubTask subtask_copy = subtask.clone();

        BOOST_FOREACH(GraspTarget& target, subtask.getTargets()) {

            GraspTarget newTarget = target;

            if (newTarget.getResult()->testStatus != GraspResult::Skip) {
                newTarget.getResult()->testStatus = GraspResult::UnInitialized;
            }

            subtask_copy.addTarget(newTarget);
        }

        filteredGrasps->addSubTask(subtask_copy);
    }

    return filteredGrasps;
}
