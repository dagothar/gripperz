/**
 * @file TaskStatistics.hpp
 * @author Adam Wolniakowski
 * @date 6-07-2015
 */

#pragma once

#include <rwlibs/task/GraspTask.hpp>

namespace gripperz {
    namespace grasps {

        /**
         * @class TaskStatistics
         * @brief A collection of tools to extract statistical information from grasp tasks.
         */
        class GraspStatistics {
        public:
            /**
             * @brief Counts tasks which have a specific grasp status.
             */
            static int countGraspsWithStatus(rwlibs::task::GraspTask::Ptr tasks, const rwlibs::task::GraspResult::TestStatus& result);
        };

    } /* grasps */
} /* gripperz */
