/* 
 * File:   WorkcellGripper.cpp
 * Author: dagothar
 * 
 * Created on October 14, 2015, 11:00 PM
 */

#include "WorkcellGripper.hpp"
#include <rw/kinematics/MovableFrame.hpp>

using namespace gripperz::models;
using namespace rw::models;
using namespace rw::kinematics;
using namespace rwsim::dynamics;

WorkcellGripper::WorkcellGripper(const std::string& name) :
Gripper(name),
_initialized(false) {
}

WorkcellGripper::~WorkcellGripper() {
}

void WorkcellGripper::initialize(WorkCell::Ptr wc, DynamicWorkCell::Ptr dwc, State& state) {
    _device = wc->findDevice(_deviceId);
    _dynamicDevice = dwc->findDevice(_dynamicDeviceId);
    _TCPFrame = wc->findFrame(_TCPFrameId);
    _movableFrame = wc->findFrame<MovableFrame>(_movableFrameId);

    _initialized = true;
}