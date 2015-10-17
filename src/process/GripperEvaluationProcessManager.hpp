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

            //! Configuration.

            struct Configuration {
                unsigned nOfGraspsPerEvaluation;
                unsigned nOfRobustnessTargets;
                double sigma_a;
                double sigma_p;

                Configuration() :
                nOfGraspsPerEvaluation(100),
                nOfRobustnessTargets(100),
                sigma_a(8.0),
                sigma_p(0.003) {
                }
            };

        public:
            GripperEvaluationProcessManager(const Configuration& configuration);

            virtual ~GripperEvaluationProcessManager();

            Configuration getConfiguration() const {
                return _config;
            }

            void setConfiguration(const Configuration& config) {
                _config = config;
            }

            virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper) = 0;

        private:
            Configuration _config;
        };

    } /* evaluation */
} /* gripperz */
