/* 
 * File:   StandardGripper.cpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 2:33 PM
 */

#include "StandardGripper.hpp"

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace gripperz::models;
using namespace gripperz::parametrization;
using namespace rw::math;
using namespace std;

StandardGripper::StandardGripper(const std::string& name) :
ParametrizedGeometryGripper(name) {
}

StandardGripper::StandardGripper(const ParametrizedGeometryGripper& gripper) :
ParametrizedGeometryGripper(gripper) {

}

StandardGripper::~StandardGripper() {
}

StandardGripper* StandardGripper::clone() const {
    Parametrization::Ptr p = this->getParametrization()->clone();
    StandardGripper* gripper = new StandardGripper(*this);
    gripper->setParametrization(p);
    
    return gripper;
}

void StandardGripper::applyModifications(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state) {
    ParametrizedGeometryGripper::applyModifications(wc, dwc, state);

    INFO << "Modifying offset, stroke, jawdist and force" << endl;

    /* set TCP */
    double tcpoffset = getParameter("tcpoffset");
    getTCPFrame()->setTransform(Transform3D<>(Vector3D<>(0, 0, tcpoffset)), state);

    /* set bounds */
    double minopening = 0.5 * getParameter("jawdist");
    double stroke = 0.5 * getParameter("stroke");

    getDevice()->setBounds(make_pair(Q(1, minopening), Q(1, minopening + stroke)));
    getDevice()->setQ(Q(1, minopening), state);

    /* set force */
    double force = getParameter("force");
    getDynamicDevice()->setMotorForceLimits(Q(2, force, force));
}
