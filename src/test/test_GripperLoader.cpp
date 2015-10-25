#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "GripperLoaderTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <models/loaders/BaseGripperLoader.hpp>

using namespace std;
using namespace gripperz::models;
using namespace gripperz::models::loaders;

BOOST_AUTO_TEST_CASE(ShouldLoadGripper) {
    BaseGripperLoader::Ptr loader = new BaseGripperLoader();
    Gripper::Ptr gripper = loader->load("../data/test/gripper.xml");
    
    BOOST_CHECK(gripper->getName() == "myLittleGripper");
    BOOST_CHECK(gripper->getDeviceId() == "gripper");
    BOOST_CHECK(gripper->getLeftFingerId() == "gripper.LeftFinger");
}

BOOST_AUTO_TEST_CASE(ShouldSaveQuality) {
    BaseGripperLoader::Ptr loader = new BaseGripperLoader();
    Gripper::Ptr gripper = loader->load("../data/test/gripper.xml");
    
    loader->save("g.xml", gripper);
    Gripper::Ptr g = loader->load("g.xml");
    
    BOOST_CHECK(g->getName() == "myLittleGripper");
    BOOST_CHECK(g->getDeviceId() == "gripper");
    BOOST_CHECK(g->getLeftFingerId() == "gripper.LeftFinger");
}