/**
 * @file GripperEvaluationManagerBuilder.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include "GripperEvaluationManager.hpp"
#include "StandardEvaluationManager.hpp"
#include <grasps/SurfaceSample.hpp>

namespace gripperz {
namespace evaluation {

/**
 * @class GripperEvaluationManagerBuilder
 * @brief
 */
class GripperEvaluationManagerBuilder {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperEvaluationManagerBuilder> Ptr;
	
public:
	GripperEvaluationManagerBuilder(context::TaskDescription::Ptr td, unsigned nThreads=1, const std::vector<grasps::SurfaceSample>& ssamples=std::vector<grasps::SurfaceSample>());
	
	virtual ~GripperEvaluationManagerBuilder();
	
	StandardEvaluationManager::Ptr build();
	
	GripperEvaluationManagerBuilder& generator(grasps::GraspSource::Ptr gen);
	GripperEvaluationManagerBuilder& simulator(simulation::GripperSimulator::Ptr sim);
	GripperEvaluationManagerBuilder& evaluator(evaluation::GripperEvaluator::Ptr eval);
	GripperEvaluationManagerBuilder& configuration(const GripperEvaluationManager::Configuration& config);
	
private:
	context::TaskDescription::Ptr _td;
	grasps::GraspSource::Ptr _generator;
	simulation::GripperSimulator::Ptr _simulator;
	evaluation::GripperEvaluator::Ptr _evaluator;
	GripperEvaluationManager::Configuration _config;
	std::vector<grasps::SurfaceSample> _ssamples;
};

} /* evaluation */
} /* gripperz */
