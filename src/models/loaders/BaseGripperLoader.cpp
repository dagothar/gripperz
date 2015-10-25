/* 
 * File:   GripperLoader.cpp
 * Author: dagothar
 * 
 * Created on October 23, 2015, 11:44 AM
 */

#include "BaseGripperLoader.hpp"

using namespace std;
using namespace rw::common;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace boost::property_tree;

BaseGripperLoader::BaseGripperLoader() {
}

BaseGripperLoader::~BaseGripperLoader() {
}

void readData(const boost::property_tree::ptree& tree, Gripper::Ptr gripper) {

    string deviceId = tree.get<string>("deviceId");
    string dynamicDeviceId = tree.get<string>("dynamicDeviceId");
    string tcpFrameId = tree.get<string>("tcpFrameId");
    string movableFrameId = tree.get<string>("movableFrameId");
    string graspControllerId = tree.get<string>("graspControllerId");
    string leftFingerId = tree.get<string>("leftFingerId");
    string rightFingerId = tree.get<string>("rightFingerId");

    gripper->setDeviceId(deviceId);
    gripper->setDynamicDeviceId(dynamicDeviceId);
    gripper->setTCPFrameId(tcpFrameId);
    gripper->setMovableFrameId(movableFrameId);
    gripper->setGraspControllerId(graspControllerId);
    gripper->setLeftFingerId(leftFingerId);
    gripper->setRightFingerId(rightFingerId);
}

Gripper::Ptr BaseGripperLoader::read(const boost::property_tree::ptree& tree) {
    string name = tree.get<string>("<xmlattr>.name");
    Gripper::Ptr gripper = ownedPtr(new Gripper(name));

    const ptree& data = tree.get_child("data");
    readData(data, gripper);

    return gripper;
}

pair<string, ptree> BaseGripperLoader::write(Gripper::Ptr gripper) {
    ptree tree;

    tree.put("<xmlattr>.name", gripper->getName());
    tree.put("data.deviceId", gripper->getDeviceId());
    tree.put("data.dynamicDeviceId", gripper->getDynamicDeviceId());
    tree.put("data.tcpFrameId", gripper->getTCPFrameId());
    tree.put("data.movableFrameId", gripper->getMovableFrameId());
    tree.put("data.graspControllerId", gripper->getGraspControllerId());
    tree.put("data.leftFingerId", gripper->getLeftFingerId());
    tree.put("data.rightFingerId", gripper->getRightFingerId());

    return make_pair("gripper", tree);
}
