/**
 * @file GripperEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 6-07-2015
 */

#pragma once

#include <models/Gripper.hpp>

namespace gripperz {
    namespace process {

        /**
         * @class GripperEvaluationManager
         * @brief Manages the process of gripper evaluation: from task planning to calculating quality.
         */
        class GripperEvaluationProcessManager {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperEvaluationProcessManager> Ptr;

        public:
            GripperEvaluationProcessManager();

            virtual ~GripperEvaluationProcessManager();

            virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper) = 0;
        };

    } /* evaluation */
} /* gripperz */
