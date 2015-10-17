/**
 * @file StandardEvaluationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#include "StandardGripperEvaluationProcessManager.hpp"
#include <grasps/filters/RobustnessGraspFilter.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::process;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace gripperz::models;
using namespace rwlibs::task;
using namespace rw::kinematics;
using namespace rw::math;
using namespace rw::common;
using namespace std;

StandardGripperEvaluationProcessManager::StandardGripperEvaluationProcessManager(
        context::TaskDescription::Ptr context,
        grasps::GraspSource::Ptr graspSource,
        simulation::GripperSimulator::Ptr simulator,
        evaluation::OldGripperEvaluator::Ptr evaluator,
        const Configuration& configuration
        ) :
GripperEvaluationProcessManager(configuration),
_context(context),
_graspSource(graspSource),
_simulator(simulator),
_evaluator(evaluator) {
}

StandardGripperEvaluationProcessManager::~StandardGripperEvaluationProcessManager() {
}

GripperQuality::Ptr StandardGripperEvaluationProcessManager::evaluateGripper(Gripper::Ptr gripper) {

    registerGripper(gripper);
    Grasps grasps = generateGrasps();
    Grasps simulatedGrasps = simulateGrasps(grasps);
    GripperQuality::Ptr quality = doEvaluation(gripper, simulatedGrasps);

    return quality;
}

void StandardGripperEvaluationProcessManager::registerGripper(models::Gripper::Ptr gripper) {
    
    gripper->registerWithContext(_context->getWorkCell(), _context->getDynamicWorkCell(), _context->getInitState());
    gripper->applyModifications(_context->getWorkCell(), _context->getDynamicWorkCell(), _context->getInitState());
}

Grasps StandardGripperEvaluationProcessManager::generateGrasps() {
    DEBUG << " --- GENERATING GRASPS ---" << endl;

    Grasps grasps = NULL;
    try {
        grasps = _graspSource->getGrasps();

    } catch (const std::exception& e) {
        RW_WARN("Exception during grasp generation! " << e.what());
    }

    return grasps;
}

Grasps StandardGripperEvaluationProcessManager::simulateGrasps(Grasps grasps) {
    DEBUG << " --- SIMULATING GRASPS ---" << endl;

    try {
        _simulator->loadTasks(grasps);
        _simulator->start(_context->getInitState());

        while (_simulator->isRunning()) {
        }
    } catch (const std::exception& e) {
        RW_THROW("Exception during grasp simulation! " << e.what());
    }

    return _simulator->getTasks();
}

GripperQuality::Ptr StandardGripperEvaluationProcessManager::doEvaluation(Gripper::Ptr gripper, Grasps grasps) {
    DEBUG << " --- EVALUATING GRIPPER & GRASPS ---" << endl;
    
    GripperQuality::Ptr quality = NULL;
    try {
        quality = _evaluator->evaluate(gripper, grasps);

    } catch (const std::exception& e) {
        RW_THROW("Exception during gripper evaluation! " << e.what());
    }
    
    return quality;
}
