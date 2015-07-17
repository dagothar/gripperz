/**
 * @file GripperEvaluationManagerFactory.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "GripperEvaluationManagerFactory.hpp"
#include <simulation/InterferenceSimulator.hpp>
#include <grasps/TaskGenerator.hpp>
#include <evaluation/GripperEvaluator.hpp>
#include "GripperEvaluationManagerBuilder.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::grasps;
using namespace gripperz::simulation;
using namespace rw::common;


GripperEvaluationManager::Ptr GripperEvaluationManagerFactory::getEvaluationManager(
	context::TaskDescription::Ptr td,
	const GripperEvaluationManager::Configuration& config,
	unsigned nThreads,
	const std::vector<grasps::SurfaceSample>& ssamples
) {
	GripperEvaluationManagerBuilder builder(td, nThreads, ssamples);
	
	GripperEvaluationManager::Ptr manager = builder
		.generator(ownedPtr(new TaskGenerator(td, ssamples)))
		.simulator(ownedPtr(new InterferenceSimulator(td->getDynamicWorkCell(), td->getInterferenceLimit(), td->getInterferenceObjects(), nThreads)))
		.evaluator(ownedPtr(new GripperEvaluator(td, td->getAlignmentCalculator())))
		.configuration(config)
		.build();
	
	return manager;
}


