/**
 * @file GripperEvaluationManagerFactory.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "GripperEvaluationManagerFactory.hpp"
#include <simulation/InterferenceSimulator.hpp>
#include <grasps/planners/BasicParallelGripperGraspPlanner.hpp>
#include <evaluation/OldGripperEvaluator.hpp>
#include "GripperEvaluationManagerBuilder.hpp"


using namespace gripperz::process;
using namespace gripperz::evaluation;
using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace gripperz::simulation;
using namespace rw::common;

StandardGripperEvaluationProcessManager::Ptr GripperEvaluationManagerFactory::makeStandardEvaluationManager(
        context::TaskDescription::Ptr td,
        unsigned nOfGraspsPerEvaluation,
        unsigned nThreads,
        const std::vector<grasps::SurfaceSample>& ssamples
        ) {
    GripperEvaluationManagerBuilder builder(td, nThreads, ssamples);

    BasicParallelGripperGraspPlanner::Ptr graspSource = ownedPtr(new BasicParallelGripperGraspPlanner(nOfGraspsPerEvaluation, td->getInitState(), td));
    graspSource->setSurfaceSamples(ssamples);

    StandardGripperEvaluationProcessManager::Ptr manager = builder
            .generator(graspSource)
            .simulator(ownedPtr(new InterferenceSimulator(td->getDynamicWorkCell(), td->getInterferenceLimit(), td->getInterferenceObjects(), nThreads)))
            .evaluator(ownedPtr(new OldGripperEvaluator(td, td->getAlignmentCalculator())))
            .build();

    return manager;
}


