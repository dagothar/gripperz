#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ParametrizedGripperLoaderTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <models/loaders/ParametrizedGripperLoader.hpp>

using namespace std;
using namespace gripperz::parametrization;
using namespace gripperz::models;
using namespace gripperz::models::loaders;

BOOST_AUTO_TEST_CASE(ShouldLoadParametrizedGripper) {
    ParametrizedGripperLoader::Ptr loader = new ParametrizedGripperLoader();
    ParametrizedGripper::Ptr gripper = loader->load("../data/test/parametrized_gripper.xml");
    
    BOOST_CHECK(gripper->getName() == "myLittleParametrizedGripper");
    BOOST_CHECK(gripper->getDeviceId() == "gripper");
    BOOST_CHECK(gripper->getLeftFingerId() == "gripper.LeftFinger");
    
    Parametrization::Ptr p = gripper->getParametrization();
    
    BOOST_CHECK_CLOSE(p->getParameter("weight"), 9001, 1e-6);
    BOOST_CHECK_CLOSE(p->getParameter("cost"), 999, 1e-6);
}

BOOST_AUTO_TEST_CASE(ShouldSaveQuality) {
    ParametrizedGripperLoader::Ptr loader = new ParametrizedGripperLoader();
    ParametrizedGripper::Ptr gripper = loader->load("../data/test/parametrized_gripper.xml");
    
    loader->save("pg.xml", gripper);
    ParametrizedGripper::Ptr g = loader->load("pg.xml");
    
    BOOST_CHECK(g->getName() == "myLittleParametrizedGripper");
    BOOST_CHECK(g->getDeviceId() == "gripper");
    BOOST_CHECK(g->getLeftFingerId() == "gripper.LeftFinger");
    
    Parametrization::Ptr p = g->getParametrization();
    
    BOOST_CHECK_CLOSE(p->getParameter("weight"), 9001, 1e-6);
    BOOST_CHECK_CLOSE(p->getParameter("cost"), 999, 1e-6);
}