/**
 * @file GripperEvaluationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "GripperEvaluationManager.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace gripperz::models;
using namespace rwlibs::task;


GripperEvaluationManager::GripperEvaluationManager(
	context::TaskDescription::Ptr context,
	grasps::TaskGenerator::Ptr generator,
	simulation::GripperSimulator::Ptr simulation,
	evaluation::GripperEvaluator::Ptr evaluator,
	const Configuration& configuration
) :
	_context(context),
	_generator(generator),
	_simulation(simulation),
	_evaluator(evaluator),
	_config(configuration)
{}


GripperEvaluationManager::~GripperEvaluationManager() {
}


GripperQuality::Ptr GripperEvaluationManager::evaluateGripper(Gripper::Ptr gripper) {
	applyGripperParametrization(gripper);
	
	/*
	 * Generate grasps.
	 */
	GraspTask::Ptr targets = NULL;
	GraspTask::Ptr samples = NULL;
	try {
		_generator->generateTasks(_config.nOfGraspsPerEvaluation, _context->getInitState());
		
		targets = _generator->getTasks();
		samples = _generator->getSamples();
	} catch (...) {
		RW_THROW("Exception during grasp generation!");
	}
	
	/*
	 * Simulate grasping.
	 */
	try {
		_simulation->loadTasks(targets);
		
		_simulation->start(_context->getInitState());
		
		while (_simulation->isRunning()) {
		}
	} catch (...) {
		RW_THROW("Exception during grasp simulation!");
	}
	
	/*
	 * Evaluate gripper.
	 */
	GripperQuality::Ptr quality = NULL;
	try {
		quality = _evaluator->evaluateGripper(gripper, targets, samples);
	} catch (...) {
		RW_THROW("Exception during gripper evaluation!");
	}
	
	return quality;
}


void GripperEvaluationManager::applyGripperParametrization(models::Gripper::Ptr gripper) {
	gripper->updateGripper(
		_context->getWorkCell(),
		_context->getDynamicWorkCell(),
		_context->getGripperDevice(),
		_context->getGripperDynamicDevice(),
		_context->getInitState(),
		_context
	);
}
	

void GripperEvaluationManager::generateTasks(models::Gripper::Ptr gripper) {
}
	

void GripperEvaluationManager::simulateGripper(models::Gripper::Ptr gripper) {
}
