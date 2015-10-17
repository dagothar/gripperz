/**
 * @file GripperEvaluationManagerFactory.hpp
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
         * @class GripperEvaluationManagerFactory
         * @brief
         */
        class GripperEvaluationManagerFactory {
        public:
            static StandardGripperEvaluationProcessManager::Ptr makeStandardEvaluationManager(
                    context::TaskDescription::Ptr td,
                    unsigned nOfGraspsPerEvaluation,
                    unsigned nThreads = 1,
                    const std::vector<grasps::SurfaceSample>& ssamples = std::vector<grasps::SurfaceSample>()
                    );
        };

    } /* evaluation */
} /* gripperz */
