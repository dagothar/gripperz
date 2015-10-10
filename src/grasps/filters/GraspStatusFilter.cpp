/* 
 * File:   GraspStatusFilter.cpp
 * Author: dagothar
 * 
 * Created on October 10, 2015, 9:18 PM
 */

#include "GraspStatusFilter.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::task;

GraspStatusFilter::GraspStatusFilter(const std::vector<GraspResult::TestStatus>& allowedStatuses) :
_allowedStatuses(allowedStatuses) {

}

GraspStatusFilter::~GraspStatusFilter() {
}

Grasps GraspStatusFilter::filter(Grasps grasps) {
    Grasps filteredGrasps = grasps->clone();

    BOOST_FOREACH(GraspSubTask& subtask, grasps->getSubTasks()) {
        GraspSubTask subtask_copy = subtask.clone();

        BOOST_FOREACH(GraspTarget& target, subtask.getTargets()) {

            if (isAllowed((GraspResult::TestStatus)target.getResult()->testStatus)) {
                subtask_copy.addTarget(target);
            }
        }

        filteredGrasps->addSubTask(subtask_copy);
    }

    return filteredGrasps;
}

void GraspStatusFilter::addAllowedStatus(GraspResult::TestStatus status) {
    _allowedStatuses.push_back(status);
}

bool GraspStatusFilter::isAllowed(GraspResult::TestStatus status) const {

    BOOST_FOREACH(GraspResult::TestStatus s, _allowedStatuses) {
        if (status == s) {
            return true;
        }
    }

    return false;
}
