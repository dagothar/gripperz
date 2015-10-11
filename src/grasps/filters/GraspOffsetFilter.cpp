/* 
 * File:   GraspOffsetFilter.cpp
 * Author: dagothar
 * 
 * Created on October 11, 2015, 10:34 PM
 */

#include "GraspOffsetFilter.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::task;

GraspOffsetFilter::GraspOffsetFilter(const rw::math::Transform3D<>& offset) :
_offset(offset) {
}

GraspOffsetFilter::~GraspOffsetFilter() {
}

Grasps GraspOffsetFilter::filter(Grasps grasps) {
    Grasps filteredGrasps = grasps->clone();

    BOOST_FOREACH(GraspSubTask& subtask, grasps->getSubTasks()) {

        GraspSubTask subtask_copy = subtask.clone();

        BOOST_FOREACH(GraspTarget& target, subtask.getTargets()) {

            GraspTarget newTarget = target;
            newTarget.pose = _offset * newTarget.pose;
            subtask_copy.addTarget(newTarget);
        }

        filteredGrasps->addSubTask(subtask_copy);
    }

    return filteredGrasps;
}
