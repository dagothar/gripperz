/**
 * @file GripperEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 6-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <context/TaskDescription.hpp>
#include <grasps/TaskGenerator.hpp>
#include <simulation/GripperSimulator.hpp>
#include <evaluation/GripperEvaluator.hpp>
#include <models/Gripper.hpp>

namespace gripperz {
namespace evaluation {

/**
 * @class GripperEvaluationManager
 * @brief Manages the process of gripper evaluation: from task planning to calculating quality.
 */
class GripperEvaluationManager {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperEvaluationManager> Ptr;
	
	//! Configuration.
	struct Configuration {
		unsigned nOfGraspsPerEvaluation;
		unsigned nOfRobustnessTargets;
		double sigma_a;
		double sigma_p;
		
		Configuration() :
			nOfGraspsPerEvaluation(100),
			nOfRobustnessTargets(100),
			sigma_a(8.0),
			sigma_p(0.003)
		{}
	};

public:
	/**
	 * @brief Constructor.
	 */
	GripperEvaluationManager(
		context::TaskDescription::Ptr context,
		grasps::TaskGenerator::Ptr generator,
		simulation::GripperSimulator::Ptr simulation,
		evaluation::GripperEvaluator::Ptr evaluator,
		const Configuration& configuration
	);
	
	virtual ~GripperEvaluationManager();
	
	context::TaskDescription::Ptr getContext() { return _context; }
	
	grasps::TaskGenerator::Ptr getGenerator() { return _generator; }
	
	simulation::GripperSimulator::Ptr getSimulator() { return _simulator; }
	
	evaluation::GripperEvaluator::Ptr getEvaluator() { return _evaluator; }
	
	Configuration getConfiguration() const { return _config; }
	void setConfiguration(const Configuration& config) { _config = config; }
	
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
	
	Configuration _config;
};

} /* evaluation */
} /* gripperz */
