/**
 * @file GraspDatabase.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-07
 */

#include "GraspDatabase.hpp"

using namespace gripperz::grasps;

Grasps GraspDatabase::getGrasps() {
    if (!_grasps) {
        _grasps = loadGrasps();
    }

    return _grasps;
}

