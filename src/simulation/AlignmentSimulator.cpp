/**
 * @file AlignmentSimulator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "AlignmentSimulator.hpp"

#include <iostream>
#include <boost/foreach.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::simulation;
using namespace rwlibs::task;
using namespace rw::models;
using namespace rw::math;
using namespace std;

AlignmentSimulator::AlignmentSimulator(
                                       rwsim::dynamics::DynamicWorkCell::Ptr dwc,
                                       double limit,
                                       unsigned nThreads,
                                       AlignmentMetric::Ptr metric
                                       ) :
BasicSimulator(dwc, nThreads),
_alignmentLimit(limit),
_metric(metric) {
}

AlignmentSimulator::~AlignmentSimulator() {
}

void AlignmentSimulator::evaluateGrasp(SimState& sstate) {
    BasicSimulator::evaluateGrasp(sstate);

    /* if the grasp is success, it may still cause too much interference */
    if (sstate._target->getResult()->testStatus == GraspResult::Success) {
        double interference = 0.0; //calculateSceneInterference(getInitState(), sstate._state);

        if (interference > _alignmentLimit) {
            DEBUG << "Grasp above interference limit." << endl;
            sstate._target->getResult()->testStatus = GraspResult::Interference;
        }
    }
}

//double AlignmentSimulator::calculateSceneInterference(const rw::kinematics::State& state0, const rw::kinematics::State& state1) const {
//    double interference = 0.0;
//
//    BOOST_FOREACH(Object::Ptr object, _interferenceObjects) {
//        interference += calculateObjectInterference(object, state0, state1);
//    }
//
//    return interference;
//}
//
//double AlignmentSimulator::calculateObjectInterference(rw::models::Object::Ptr object, const rw::kinematics::State& state0, const rw::kinematics::State& state1) const {
//    Transform3D<> Tbefore = object->getBase()->getTransform(state0);
//    Transform3D<> Tafter = object->getBase()->getTransform(state1);
//
//    double objInt = _metric->distance(Tbefore, Tafter);
//
//    //INFO << "Interference " << object->getName() << ": " << objInt << endl;
//
//    return objInt;
//}
