/* 
 * File:   GripperFactory.cpp
 * Author: dagothar
 * 
 * Created on October 16, 2015, 11:01 PM
 */

#include "GripperFactory.hpp"
#include "ParallelFingerGripper.hpp"
#include "OldGripper.hpp"

using namespace gripperz::models;
using namespace rw::common;

GripperFactory::GripperFactory() {
}

GripperFactory::~GripperFactory() {
}

WorkcellGripper::Ptr GripperFactory::makeGripper(const std::string& classId, const std::string& name) {
    if (classId == "WorkcellGripper") {
        return ownedPtr(new WorkcellGripper(name));
    } else if (classId == "ParallelFingerGripper") {
        return ownedPtr(new ParallelFingerGripper(name));
    } else if (classId == "OldGripper") {
        return ownedPtr(new OldGripper(name));
    }
    
    RW_THROW("Gripper class not supported!");
    return NULL; // for warnings
}
