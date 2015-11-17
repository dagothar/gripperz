/* 
 * File:   ParametrizedGripper.cpp
 * Author: dagothar
 * 
 * Created on October 22, 2015, 8:47 AM
 */

#include "ParametrizedGripper.hpp"

using namespace gripperz::models;
using namespace gripperz::parametrization;

ParametrizedGripper::ParametrizedGripper(const std::string& name, parametrization::Parametrization::Ptr parametrization) :
Gripper(name),
Parametrized(parametrization) {

}

ParametrizedGripper::ParametrizedGripper(const Gripper& gripper, parametrization::Parametrization::Ptr parametrization) :
Gripper(gripper),
Parametrized(parametrization) {

}

ParametrizedGripper::~ParametrizedGripper() {
}

ParametrizedGripper* ParametrizedGripper::clone() const {
    Parametrization::Ptr p = this->getParametrization()->clone();
    ParametrizedGripper* gripper = new ParametrizedGripper(*this);
    gripper->setParametrization(p);
    
    return gripper;
}

