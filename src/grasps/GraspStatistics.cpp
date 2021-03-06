/**
 * @file TaskStatistics.cpp
 * @author Adam Wolniakowski
 * @date 6-07-2015
 */

#include "GraspStatistics.hpp"


using namespace rwlibs::task;
using namespace gripperz::grasps;

int GraspStatistics::countGraspsWithStatus(rwlibs::task::GraspTask::Ptr tasks, const rwlibs::task::GraspResult::TestStatus& result) {
    int n = 0;

    typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget p, tasks->getAllTargets()) {
        if (p.second->getResult()->testStatus == result) {
            ++n;
        }
    }

    return n;
}

int GraspStatistics::countGraspTargets(Grasps grasps) {
    return grasps->getAllTargets().size();
}
