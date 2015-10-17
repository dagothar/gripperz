/**
 * @file GripperEvaluationManagerBuilder.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include "GripperEvaluationProcessManager.hpp"
#include "StandardGripperEvaluationProcessManager.hpp"
#include <grasps/SurfaceSample.hpp>

namespace gripperz {
    namespace process {

        /**
         * @class GripperEvaluationManagerBuilder
         * @brief
         */
        class GripperEvaluationManagerBuilder {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperEvaluationManagerBuilder> Ptr;

        public:
            GripperEvaluationManagerBuilder(context::TaskDescription::Ptr td, unsigned nThreads = 1, const std::vector<grasps::SurfaceSample>& ssamples = std::vector<grasps::SurfaceSample>());

            virtual ~GripperEvaluationManagerBuilder();

            StandardGripperEvaluationProcessManager::Ptr build();

            GripperEvaluationManagerBuilder& generator(grasps::GraspSource::Ptr gen);
            GripperEvaluationManagerBuilder& simulator(simulation::GripperSimulator::Ptr sim);
            GripperEvaluationManagerBuilder& evaluator(evaluation::OldGripperEvaluator::Ptr eval);

        private:
            context::TaskDescription::Ptr _td;
            grasps::GraspSource::Ptr _generator;
            simulation::GripperSimulator::Ptr _simulator;
            evaluation::OldGripperEvaluator::Ptr _evaluator;
            std::vector<grasps::SurfaceSample> _ssamples;
        };

    } /* evaluation */
} /* gripperz */
