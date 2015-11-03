/* 
 * File:   StandardGripper.cpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 2:33 PM
 */

#include "StandardGripper.hpp"

using namespace gripperz::models;

StandardGripper::StandardGripper(const std::string& name) :
BasicParametrizedGripper(name),
ParametrizedGeometryGripper(name) {
}

StandardGripper::StandardGripper(const ParametrizedGeometryGripper& gripper) :
BasicParametrizedGripper(gripper),
ParametrizedGeometryGripper(gripper) {
    
}

StandardGripper::~StandardGripper() {
}

void StandardGripper::applyModifications(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state) {
    ParametrizedGeometryGripper::applyModifications(wc, dwc, state);
    BasicParametrizedGripper::applyModifications(wc, dwc, state);
}
