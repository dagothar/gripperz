/* 
 * File:   RWGraspDatabase.cpp
 * Author: dagothar
 * 
 * Created on October 9, 2015, 9:51 AM
 */

#include "RWGraspDatabase.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::databases;
using namespace rwlibs::task;

RWGraspDatabase::RWGraspDatabase(const std::string& filename) :
_filename(filename) {
}

RWGraspDatabase::~RWGraspDatabase() {
}

Grasps RWGraspDatabase::loadGrasps() {
    Grasps grasps = GraspTask::load(_filename);
    
    return grasps;
}
