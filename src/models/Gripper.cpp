/* 
 * File:   Gripper.cpp
 * Author: dagothar
 * 
 * Created on October 12, 2015, 2:20 PM
 */

#include "Gripper.hpp"
#include "GripperQualityFactory.hpp"

using namespace gripperz::models;

Gripper::Gripper(const std::string& name) :
_name(name) {
    this->_quality = GripperQualityFactory::makeGripperQuality();
}

Gripper::~Gripper() {
}

