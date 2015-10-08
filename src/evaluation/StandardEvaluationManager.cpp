/**
 * @file StandardEvaluationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#include "StandardEvaluationManager.hpp"

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace gripperz::models;
using namespace rwlibs::task;
using namespace rw::kinematics;
using namespace rw::math;
using namespace rw::common;
using namespace std;


StandardEvaluationManager::StandardEvaluationManager(
	context::TaskDescription::Ptr context,
	grasps::GraspSource::Ptr generator,
	simulation::GripperSimulator::Ptr simulator,
	evaluation::GripperEvaluator::Ptr evaluator,
	const Configuration& configuration
) :
	GripperEvaluationManager(configuration),
	_context(context),
	_generator(generator),
	_simulator(simulator),
	_evaluator(evaluator)
{
}


StandardEvaluationManager::~StandardEvaluationManager() {
}


GripperQuality::Ptr StandardEvaluationManager::evaluateGripper(Gripper::Ptr gripper) {
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
	GraspTask::Ptr samples = NULL;
	try {
		DEBUG << "Planning tasks" << endl;
		_generator->generateTasks(config.nOfGraspsPerEvaluation, state);
		
		targets = _generator->getGrasps();
		samples = _generator->getSamples();
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
			rtargets = GraspSource::copyTasks(targets, true);
			rtargets = GraspSource::generateRobustnessTasks(rtargets, config.nOfRobustnessTargets, config.sigma_p, config.sigma_a * Deg2Rad);
			
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
		quality = _evaluator->evaluateGripper(gripper, targets, samples, rtargets);
	} catch (const std::exception& e) {
		RW_THROW("Exception during gripper evaluation! " << e.what());
	}
	
	return quality;
}


void StandardEvaluationManager::applyGripperParametrization(models::Gripper::Ptr gripper, rw::kinematics::State& state) {
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
	

void StandardEvaluationManager::generateTasks(models::Gripper::Ptr gripper) {
}
	

void StandardEvaluationManager::simulateGripper(models::Gripper::Ptr gripper) {
}
