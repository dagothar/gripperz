/**
 * @file InterferenceSimulator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "InterferenceSimulator.hpp"

#include <iostream>
#include <boost/foreach.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::simulator;
using namespace rwlibs::task;
using namespace rw::models;
using namespace rw::math;
using namespace std;


InterferenceSimulator::InterferenceSimulator(
	rwsim::dynamics::DynamicWorkCell::Ptr dwc,
	double limit,
	const std::vector<rw::models::Object::Ptr>& objects,
	InterferenceMetric::Ptr metric,
	unsigned threads
) :
	BasicSimulator(dwc, threads),
	_interferenceLimit(limit),
	_interferenceObjects(objects),
	_metric(metric)
{
}


InterferenceSimulator::~InterferenceSimulator() {
}


void InterferenceSimulator::evaluateGrasp(SimState& sstate) {
	BasicSimulator::evaluateGrasp(sstate);
	
	double interference = calculateSceneInterference(getInitState(), sstate._state);
	
	if (interference > _interferenceLimit) {
		DEBUG << "Grasp above interference limit." << endl;
		sstate._target->getResult()->testStatus = GraspResult::Interference;
	}
}


double InterferenceSimulator::calculateSceneInterference(const rw::kinematics::State& state0, const rw::kinematics::State& state1) const {
	double interference = 0.0;
	
	BOOST_FOREACH (Object::Ptr object, _interferenceObjects) {
		interference += calculateObjectInterference(object, state0, state1);
	}
	
	return interference;
}


double InterferenceSimulator::calculateObjectInterference(rw::models::Object::Ptr object, const rw::kinematics::State& state0, const rw::kinematics::State& state1) const {
	Transform3D<> Tbefore = object->getBase()->getTransform(state0);
	Transform3D<> Tafter = object->getBase()->getTransform(state1);

	double objInt = _metric->distance(Tbefore, Tafter);
	
	return objInt;
}
