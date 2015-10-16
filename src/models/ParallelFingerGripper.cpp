/* 
 * File:   ParallelFingerGripper.cpp
 * Author: dagothar
 * 
 * Created on October 16, 2015, 10:35 PM
 */

#include "ParallelFingerGripper.hpp"

using namespace gripperz::models;

ParallelFingerGripper::ParallelFingerGripper(const std::string& name) :
WorkcellGripper(name) {

}

ParallelFingerGripper::~ParallelFingerGripper() {
}

void ParallelFingerGripper::initialize(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state) {
    WorkcellGripper::initialize(wc, dwc, state);
}
