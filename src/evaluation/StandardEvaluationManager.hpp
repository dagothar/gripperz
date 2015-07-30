/**
 * @file StandardEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#pragma once

#include "GripperEvaluationManager.hpp"

namespace gripperz {
namespace evaluation {

/**
 * @class StandardEvaluationManager
 * @brief Uses standard evaluation (in the same process).
 */
class StandardEvaluationManager: public GripperEvaluationManager {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<StandardEvaluationManager> Ptr;

public:
	StandardEvaluationManager(
		context::TaskDescription::Ptr context,
		grasps::TaskGenerator::Ptr generator,
		simulation::GripperSimulator::Ptr simulator,
		evaluation::GripperEvaluator::Ptr evaluator,
		const Configuration& configuration
	);
	
	virtual ~StandardEvaluationManager();
	
	context::TaskDescription::Ptr getContext() { return _context; }
	
	grasps::TaskGenerator::Ptr getGenerator() { return _generator; }
	void setSimulator(grasps::TaskGenerator::Ptr gen) { _generator = gen; }
	
	simulation::GripperSimulator::Ptr getSimulator() { return _simulator; }
	void setSimulator(simulation::GripperSimulator::Ptr sim) { _simulator = sim; }
	
	evaluation::GripperEvaluator::Ptr getEvaluator() { return _evaluator; }
	void setEvaluator(evaluation::GripperEvaluator::Ptr evaluator) { _evaluator = evaluator; }
	
	virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper);

protected:
	virtual void applyGripperParametrization(models::Gripper::Ptr gripper, rw::kinematics::State& state);
	
	virtual void generateTasks(models::Gripper::Ptr gripper);
	
	virtual void simulateGripper(models::Gripper::Ptr gripper);

private:
	context::TaskDescription::Ptr _context;
	grasps::TaskGenerator::Ptr _generator;
	simulation::GripperSimulator::Ptr _simulator;
	evaluation::GripperEvaluator::Ptr _evaluator;

};

} /* evaluation */
} /* gripperz */
