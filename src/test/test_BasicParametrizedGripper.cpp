#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "BasicParametrizedGripperTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <rwsim/loaders/DynamicWorkCellLoader.hpp>
#include <context/Context.hpp>
#include <models/BasicParametrizedGripper.hpp>
#include <parametrization/ParametrizationTranslator.hpp>

using namespace std;
using namespace gripperz::parametrization;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace rwsim::dynamics;
using namespace rwsim::loaders;
using namespace rw::common;

struct Fixture {

    Fixture() {
        /* load data */
        DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load("../data/rotor/Scene.dwc.xml");
        context = ownedPtr(new Context(dwc));

        /* create basic gripper */
        gripper = ownedPtr(new BasicParametrizedGripper("gripper"));
        gripper->setDeviceId("gripper");
        gripper->setDynamicDeviceId("gripper");
        gripper->setTCPFrameId("TCPgripper");
        gripper->setMovableFrameId("gripper.Base");
        gripper->setLeftFingerId("gripper.LeftFinger");
        gripper->setRightFingerId("gripper.RightFinger");
        
        gripper->registerWithContext(context->getWorkCell(), context->getDynamicWorkCell(), context->getInitState());
    }

    ~Fixture() {
    }

    Context::Ptr context;
    ParametrizedGripper::Ptr gripper;
};

BOOST_AUTO_TEST_CASE(ShouldHaveBasicParametrization) {
    ParametrizedGripper::Ptr gripper = new BasicParametrizedGripper("gripper");

    vector<string> expected{"tcpoffset", "stroke", "jawdist", "force"};
    Parametrization::ParameterNameList names = gripper->getParametrization()->getParameterNameList();

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), names.begin(), names.end());
}

BOOST_FIXTURE_TEST_CASE(ShouldModifyWorkCell, Fixture) {
    gripper->setParameter("tcpoffset", 0.09);
    gripper->setParameter("stroke", 0.05);
    gripper->setParameter("jawdist", 0.01);
    gripper->setParameter("force", 100.0);
    
    rw::kinematics::State state = context->getInitState();
    gripper->applyModifications(context->getWorkCell(), context->getDynamicWorkCell(), state);
    
    double tcp_z = gripper->getTCPFrame()->getTransform(state).P()[2];
    BOOST_CHECK(tcp_z == 0.09);
    
    double min_q = gripper->getDevice()->getBounds().first(0);
    double max_q = gripper->getDevice()->getBounds().second(0);
    
    BOOST_CHECK_CLOSE(min_q, 0.005, 1e-5);
    BOOST_CHECK_CLOSE(max_q, 0.03, 1e-5);
    
    double force = gripper->getDynamicDevice()->getMotorForceLimits()[0];
    BOOST_CHECK(force == 100.0);
}