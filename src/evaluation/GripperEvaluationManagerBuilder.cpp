/**
 * @file GripperEvaluationManagerBuilder.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-13
 */

#include "GripperEvaluationManagerBuilder.hpp"
#include <simulation/InterferenceSimulator.hpp>
#include <grasps/GraspSource.hpp>
#include <evaluation/GripperEvaluator.hpp>
#include <grasps/planners/BasicParallelGripperGraspPlanner.hpp>
#include "StandardEvaluationManager.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::context;
using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace gripperz::simulation;
using namespace rw::common;


GripperEvaluationManagerBuilder::GripperEvaluationManagerBuilder(context::TaskDescription::Ptr td, unsigned nThreads, const std::vector<grasps::SurfaceSample>& ssamples) :
	_td(td),
	_generator(ownedPtr(new BasicParallelGripperGraspPlanner(100, td->getInitState(), td))),
	_simulator(ownedPtr(new InterferenceSimulator(td->getDynamicWorkCell(), td->getInterferenceLimit(), td->getInterferenceObjects(), nThreads))),
	_evaluator(ownedPtr(new GripperEvaluator(td, td->getAlignmentCalculator())))
{
}


GripperEvaluationManagerBuilder::~GripperEvaluationManagerBuilder() {
}


StandardEvaluationManager::Ptr GripperEvaluationManagerBuilder::build() {
	return ownedPtr(new StandardEvaluationManager(_td, _generator, _simulator, _evaluator, _config));
}

	
GripperEvaluationManagerBuilder& GripperEvaluationManagerBuilder::generator(grasps::GraspSource::Ptr gen) {
	_generator = gen;
	
	return *this;
}


GripperEvaluationManagerBuilder& GripperEvaluationManagerBuilder::simulator(simulation::GripperSimulator::Ptr sim) {
	_simulator = sim;
	
	return *this;
}


GripperEvaluationManagerBuilder& GripperEvaluationManagerBuilder::evaluator(evaluation::GripperEvaluator::Ptr eval) {
	_evaluator = eval;
	
	return *this;
}


GripperEvaluationManagerBuilder& GripperEvaluationManagerBuilder::configuration(const GripperEvaluationManager::Configuration& config) {
	_config = config;
	
	return *this;
}
