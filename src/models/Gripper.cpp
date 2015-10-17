/* 
 * File:   Gripper.cpp
 * Author: dagothar
 * 
 * Created on October 12, 2015, 2:20 PM
 */

#include "Gripper.hpp"
#include "GripperQualityFactory.hpp"

using namespace gripperz::models;
using namespace rw::models;
using namespace rw::kinematics;
using namespace rwsim::dynamics;

Gripper::Gripper(const std::string& name) :
_name(name) {
    this->_quality = GripperQualityFactory::makeGripperQuality();
}

Gripper::~Gripper() {
}

void Gripper::registerWithContext(WorkCell::Ptr wc, DynamicWorkCell::Ptr dwc, State& state) {
    _device = wc->findDevice(_deviceId);
    _dynamicDevice = dwc->findDevice(_dynamicDeviceId);
    _TCPFrame = wc->findFrame(_TCPFrameId);
    _movableFrame = wc->findFrame<MovableFrame>(_movableFrameId);
    _leftFingerObject = wc->findObject(_leftFingerId);
    _rightFingerObject = wc->findObject(_rightFingerId);
    _leftFingerBody = dwc->findBody(_leftFingerId);
    _rightFingerBody = dwc->findBody(_rightFingerId);
}