/**
 * @file GripperEvaluationManagerFactory.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "GripperEvaluationManagerFactory.hpp"
#include <simulation/InterferenceSimulator.hpp>
#include <grasps/planners/BasicParallelGripperGraspPlanner.hpp>
#include <evaluation/GripperEvaluator.hpp>
#include "GripperEvaluationManagerBuilder.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace gripperz::simulation;
using namespace rw::common;

StandardEvaluationManager::Ptr GripperEvaluationManagerFactory::makeStandardEvaluationManager(
        context::TaskDescription::Ptr td,
        const GripperEvaluationManager::Configuration& config,
        unsigned nThreads,
        const std::vector<grasps::SurfaceSample>& ssamples
        ) {
    GripperEvaluationManagerBuilder builder(td, nThreads, ssamples);

    BasicParallelGripperGraspPlanner::Ptr graspSource =
            ownedPtr(new BasicParallelGripperGraspPlanner(config.nOfGraspsPerEvaluation, td->getInitState(), td));
    graspSource->setSurfaceSamples(ssamples);

    StandardEvaluationManager::Ptr manager = builder
            .generator(graspSource)
            .simulator(ownedPtr(new InterferenceSimulator(td->getDynamicWorkCell(), td->getInterferenceLimit(), td->getInterferenceObjects(), nThreads)))
            .evaluator(ownedPtr(new GripperEvaluator(td, td->getAlignmentCalculator())))
            .configuration(config)
            .build();

    return manager;
}


