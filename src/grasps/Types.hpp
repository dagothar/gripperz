/**
 * @file Types.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#pragma once

#include <rwlibs/task/GraspTask.hpp>

namespace gripperz {
    namespace grasps {

        //! A type for grasp set
        typedef rwlibs::task::GraspTask::Ptr Grasps;

        //! Copies grasp set
        Grasps copyGrasps(const Grasps tasks, bool onlySuccesses = false);

    } /* grasps */
} /* gripperz */
