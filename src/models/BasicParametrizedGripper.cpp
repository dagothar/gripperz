/* 
 * File:   BasicParametrizedGripper.cpp
 * Author: dagothar
 * 
 * Created on October 22, 2015, 9:46 AM
 */

#include "BasicParametrizedGripper.hpp"

using namespace std;
using namespace gripperz::models;
using namespace gripperz::parametrization;
using namespace rw::common;
using namespace rw::math;

BasicParametrizedGripper::BasicParametrizedGripper(const std::string& name) :
ParametrizedGripper(name) {
    setParametrization(ownedPtr(new Parametrization(Parametrization::ParameterNameList{
                                                    "tcpoffset",
                                                    "stroke",
                                                    "jawdist",
                                                    "force"
    })));
}

BasicParametrizedGripper::BasicParametrizedGripper(const std::string& name, parametrization::Parametrization::Ptr parametrization) :
ParametrizedGripper(name, parametrization) {

}

BasicParametrizedGripper::BasicParametrizedGripper(const ParametrizedGripper& gripper) :
ParametrizedGripper(gripper) {
}

BasicParametrizedGripper::~BasicParametrizedGripper() {
}

void BasicParametrizedGripper::applyModifications(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state) {
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
