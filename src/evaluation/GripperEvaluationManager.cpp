/**
 * @file GripperEvaluationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "GripperEvaluationManager.hpp"

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
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


GripperEvaluationManager::GripperEvaluationManager(
	context::TaskDescription::Ptr context,
	grasps::TaskGenerator::Ptr generator,
	simulation::GripperSimulator::Ptr simulation,
	evaluation::GripperEvaluator::Ptr evaluator,
	const Configuration& configuration
) :
	_context(context),
	_generator(generator),
	_simulator(simulation),
	_evaluator(evaluator),
	_config(configuration)
{}


GripperEvaluationManager::~GripperEvaluationManager() {
}


GripperQuality::Ptr GripperEvaluationManager::evaluateGripper(Gripper::Ptr gripper) {
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
		_generator->generateTasks(_config.nOfGraspsPerEvaluation, state);
		
		targets = _generator->getTasks();
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
	if (_config.nOfRobustnessTargets != 0) {
		DEBUG << " --- SIMULATING ROBUSTNESS ---" << endl;
		try {
			rtargets = TaskGenerator::copyTasks(targets, true);
			rtargets = TaskGenerator::addPerturbations(rtargets, _config.sigma_p,	_config.sigma_a * Deg2Rad, _config.nOfRobustnessTargets);
			
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


void GripperEvaluationManager::applyGripperParametrization(models::Gripper::Ptr gripper, rw::kinematics::State& state) {
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
	

void GripperEvaluationManager::generateTasks(models::Gripper::Ptr gripper) {
}
	

void GripperEvaluationManager::simulateGripper(models::Gripper::Ptr gripper) {
}
