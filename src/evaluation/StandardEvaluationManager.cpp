/**
 * @file StandardEvaluationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#include "StandardEvaluationManager.hpp"
#include <grasps/filters/RobustnessGraspFilter.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
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

StandardEvaluationManager::StandardEvaluationManager(
        context::TaskDescription::Ptr context,
        grasps::GraspSource::Ptr graspSource,
        simulation::GripperSimulator::Ptr simulator,
        evaluation::GripperEvaluator::Ptr evaluator,
        const Configuration& configuration
        ) :
GripperEvaluationManager(configuration),
_context(context),
_graspSource(graspSource),
_simulator(simulator),
_evaluator(evaluator) {
}

StandardEvaluationManager::~StandardEvaluationManager() {
}

GripperQuality::Ptr StandardEvaluationManager::evaluateGripper(OldGripper::Ptr gripper) {
    Configuration config = getConfiguration();

    /*
     * First, check if gripper design is sane.
     */
    if (!_evaluator->isSane(gripper)) {
        RW_WARN("Gripper design is NOT sane!");

        return ownedPtr(new GripperQuality);
    }

    State state = _context->getInitState();
    applyGripperParametrization(gripper, state);

    /*
     * Generate grasps.
     */
    GraspTask::Ptr targets = NULL;
    try {
        targets = _graspSource->getGrasps();
        
    } catch (const std::exception& e) {
        RW_WARN("Exception during grasp generation! " << e.what());
    }

    /*
     * Simulate grasping.
     */
    DEBUG << " --- SIMULATING TASKS ---" << endl;
    try {
        _simulator->loadTasks(targets);

        _simulator->start(state);

        while (_simulator->isRunning()) {
        }
    } catch (const std::exception& e) {
        RW_THROW("Exception during grasp simulation! " << e.what());
    }

    /*
     * Simulate grasps with noise for robustness.
     */
    GraspTask::Ptr rtargets = NULL;
    if (config.nOfRobustnessTargets != 0) {
        DEBUG << " --- SIMULATING ROBUSTNESS ---" << endl;
        try {
            GraspFilter::Ptr robustnessFilter = new RobustnessGraspFilter(config.nOfRobustnessTargets, config.sigma_p, config.sigma_a * Deg2Rad);
            rtargets = copyGrasps(targets, true);
            rtargets = robustnessFilter->filter(rtargets);

            _simulator->loadTasks(rtargets);

            _simulator->start(state);

            while (_simulator->isRunning()) {
            }
        } catch (const std::exception& e) {
            RW_THROW("Exception during grasp simulation for robustness: " << e.what());
        }
    }

    /*
     * Evaluate gripper.
     */
    GripperQuality::Ptr quality = NULL;
    try {
        quality = _evaluator->evaluateGripper(gripper, targets, rtargets);
        
    } catch (const std::exception& e) {
        RW_THROW("Exception during gripper evaluation! " << e.what());
    }

    return quality;
}

void StandardEvaluationManager::applyGripperParametrization(models::OldGripper::Ptr gripper, rw::kinematics::State& state) {
    gripper->updateGripper(
            _context->getWorkCell(),
            _context->getDynamicWorkCell(),
            _context->getGripperDevice(),
            _context->getGripperDynamicDevice(),
            state,
            _context
            );

    DEBUG << "Updated gripper" << endl;
}