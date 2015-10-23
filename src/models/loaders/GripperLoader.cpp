/* 
 * File:   GripperLoader.cpp
 * Author: dagothar
 * 
 * Created on October 23, 2015, 11:44 AM
 */

#include "GripperLoader.hpp"

using namespace std;
using namespace rw::common;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace boost::property_tree;

GripperLoader::GripperLoader() {
}

GripperLoader::~GripperLoader() {
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

Gripper::Ptr readGripper(const boost::property_tree::ptree& tree) {
    string name = tree.get<string>("<xmlattr>.name");
    Gripper::Ptr gripper = ownedPtr(new Gripper(name));

    const ptree& data = tree.get_child("data");
    readData(data, gripper);

    return gripper;
}

Gripper::Ptr GripperLoader::read(const boost::property_tree::ptree& tree) {
    ptree root = tree.get_child("gripper");

    return readGripper(root);
}

boost::property_tree::ptree GripperLoader::write(Gripper::Ptr gripper) {
    ptree tree;

    tree.put("gripper.<xmlattr>.name", gripper->getName());
    tree.put("gripper.data.deviceId", gripper->getDeviceId());
    tree.put("gripper.data.dynamicDeviceId", gripper->getDynamicDeviceId());
    tree.put("gripper.data.tcpFrameId", gripper->getTCPFrameId());
    tree.put("gripper.data.movableFrameId", gripper->getMovableFrameId());
    tree.put("gripper.data.graspControllerId", gripper->getGraspControllerId());
    tree.put("gripper.data.leftFingerId", gripper->getLeftFingerId());
    tree.put("gripper.data.rightFingerId", gripper->getRightFingerId());

    return tree;
}
