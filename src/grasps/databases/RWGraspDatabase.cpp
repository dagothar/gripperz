/* 
 * File:   RWGraspDatabase.cpp
 * Author: dagothar
 * 
 * Created on October 9, 2015, 9:51 AM
 */

#include "RWGraspDatabase.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::databases;

RWGraspDatabase::RWGraspDatabase(const std::string& filename) :
_filename(filename) {
}

RWGraspDatabase::~RWGraspDatabase() {
}

Grasps RWGraspDatabase::getGrasps() {
    return NULL;
}

Grasps RWGraspDatabase::getSamples() {
    return NULL;
}
