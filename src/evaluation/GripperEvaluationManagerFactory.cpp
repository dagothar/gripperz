/**
 * @file GripperEvaluationManagerFactory.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "GripperEvaluationManagerFactory.hpp"
#include <simulation/InterferenceSimulator.hpp>
#include <grasps/TaskGenerator.hpp>
#include <evaluation/GripperEvaluator.hpp>


using namespace gripperz::evaluation;
using namespace gripperz::grasps;
using namespace gripperz::simulation;
using namespace rw::common;


GripperEvaluationManager::Ptr GripperEvaluationManagerFactory::getEvaluationManager(context::TaskDescription::Ptr td, unsigned nTargets) {
	TaskGenerator::Ptr generator = ownedPtr(new TaskGenerator(td));
	
	GripperSimulator::Ptr simulator = ownedPtr(new InterferenceSimulator(td->getDynamicWorkCell(), td->getInterferenceLimit(), td->getInterferenceObjects()));
	
	GripperEvaluator::Ptr evaluator = ownedPtr(new GripperEvaluator(td));
	
	GripperEvaluationManager::Configuration config;
	config.nOfGraspsPerEvaluation = nTargets;
	
	return ownedPtr(new GripperEvaluationManager(td, generator, simulator, evaluator, config));
}


