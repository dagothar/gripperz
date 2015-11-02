/* 
 * File:   ParallelGripperGraspPlanner.cpp
 * Author: dagothar
 *
 * Created on November 2, 2015, 9:33 AM
 */

#include "ParallelGripperGraspPlanner.hpp"
#include <rw/math/Transform3D.hpp>
#include <rw/kinematics/State.hpp>
#include <rw/kinematics/MovableFrame.hpp>
#include <rw/kinematics/Kinematics.hpp>

using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace rw::math;
using namespace rw::kinematics;

ParallelGripperGraspPlanner::ParallelGripperGraspPlanner(unsigned nGrasps):
GraspPlanner(nGrasps){
}

ParallelGripperGraspPlanner::~ParallelGripperGraspPlanner() {
}

void moveFrameW(const Transform3D<>& wTtcp, Frame* tcp, MovableFrame* base, State& state) {
    Transform3D<> tcpTbase = Kinematics::frameTframe(tcp, base, state);
    Transform3D<> wTbase_target = wTtcp * tcpTbase;

    base->moveTo(wTbase_target, state);
}

Grasps ParallelGripperGraspPlanner::planGrasps(unsigned nGrasps) {
    return NULL;
}
