/**
 * @file BasicSimulator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "BasicSimulator.hpp"

#include <iostream>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::simulation;
using namespace rwlibs::task;
using namespace std;

BasicSimulator::BasicSimulator(rwsim::dynamics::DynamicWorkCell::Ptr dwc, unsigned threads) :
GraspTaskSimulator(dwc, threads),
_threads(threads) {
}

BasicSimulator::~BasicSimulator() {
}

void BasicSimulator::loadTasks(rwlibs::task::GraspTask::Ptr tasks) {
    GraspTaskSimulator::load(tasks);
}

rwlibs::task::GraspTask::Ptr BasicSimulator::getTasks() {
    return GraspTaskSimulator::getTasks();
}

void BasicSimulator::start(const rw::kinematics::State& initState) {
    _initState = initState;

    //GraspTaskSimulator::init(_dwc, initState);
    GraspTaskSimulator::startSimulation(initState);
}

void BasicSimulator::stop() {
    GraspTaskSimulator::pauseSimulation();
}

bool BasicSimulator::isRunning() {
    return GraspTaskSimulator::isRunning();
}

unsigned BasicSimulator::getNrTasksDone() const {
    return const_cast<BasicSimulator*> (this)->GraspTaskSimulator::getNrTargetsDone();
}

std::vector<rwsim::simulator::ThreadSimulator::Ptr> BasicSimulator::getSimulators() {
    return GraspTaskSimulator::getSimulators();
}

void BasicSimulator::graspFinished(SimState& sstate) {
    evaluateGrasp(sstate);

    printGraspResult(sstate);
}

void BasicSimulator::evaluateGrasp(SimState& sstate) {
    /* count slippages as successes */
    if (sstate._target->getResult()->testStatus == GraspResult::ObjectSlipped) {
        sstate._target->getResult()->testStatus = GraspResult::Success;
    }
}

void BasicSimulator::printGraspResult(SimState& sstate) {
    INFO << "Grasp result " << getNrTargetsDone() << ": "
            << GraspTask::toString((GraspTask::TestStatus) sstate._target->getResult()->testStatus)
            << endl;
}
