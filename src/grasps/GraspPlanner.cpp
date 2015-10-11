/* 
 * File:   GraspPlanner.cpp
 * Author: dagothar
 * 
 * Created on October 8, 2015, 3:50 PM
 */

#include "GraspPlanner.hpp"

using namespace gripperz::grasps;

GraspPlanner::GraspPlanner(unsigned nGrasps) :
_numberOfGraspsToPlan(nGrasps) {
}

GraspPlanner::~GraspPlanner() {
}

Grasps GraspPlanner::getGrasps() {
    if (!_grasps) {
        _grasps = planGrasps(_numberOfGraspsToPlan);
    }

    return _grasps;
}