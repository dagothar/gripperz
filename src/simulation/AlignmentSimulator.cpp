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
                                       const Transform3D<>& expectedPose,
                                       double limit,
                                       unsigned nThreads,
                                       AlignmentMetric::Ptr metric
                                       ) :
BasicSimulator(dwc, nThreads),
_expectedPose(expectedPose),
_alignmentThreshold(limit),
_metric(metric) {
}

AlignmentSimulator::~AlignmentSimulator() {
}

void AlignmentSimulator::evaluateGrasp(SimState& sstate) {
    BasicSimulator::evaluateGrasp(sstate);

    /* if the grasp is success, it may still be misaligned */
    if (sstate._target->getResult()->testStatus == GraspResult::Success) {
        double misalignment = calculateAlignmentDifference(sstate);

        if (misalignment > _alignmentThreshold) {
            DEBUG << "Grasp above alignment threshold." << endl;
            sstate._target->getResult()->testStatus = GraspResult::Interference;
        }

        INFO << "Expected pose: " << _expectedPose << endl;
        INFO << "Pose: " << sstate._target->getResult()->objectTtcpLift << endl;
        INFO << "misalignment = " << misalignment << endl;
    }

}

double AlignmentSimulator::calculateAlignmentDifference(SimState& sstate) {
    Transform3D<> pose = sstate._target->getResult()->objectTtcpLift;
    double diff = _metric->distance(_expectedPose, pose);
    return diff;
}

void AlignmentSimulator::printGraspResult(SimState& sstate) {
    BasicSimulator::printGraspResult(sstate);

    //INFO << "Pose: " << sstate._target->getResult()->objectTtcpLift << endl;
}
