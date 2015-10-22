/* 
 * File:   BasicParametrizedGripper.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 9:45 AM
 */

#pragma once

#include "ParametrizedGripper.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class BasicParametrizedGripper
         * @brief Applies basic gripper parametrization
         * Basic parametrization contains following parameters:
         * - "tcpoffset" - controls the TCP frame offset from the base
         * - "stroke" - maximum gripper opening
         * - "jawdist" - minimum gripper opening
         * - "force" - closing force
         */
        class BasicParametrizedGripper : public ParametrizedGripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<BasicParametrizedGripper> Ptr;

        public:
            /**
             * Constructor.
             * Constructs the gripper with minimal required parametrization.
             * @param name
             */
            BasicParametrizedGripper(const std::string& name);

            /**
             * Constructor.
             * Constructs the gripper with custom parametrization.
             * @todo merge parametrizations?
             * @param name
             * @param parametrization
             */
            BasicParametrizedGripper(const std::string& name, parametrization::Parametrization::Ptr parametrization);

            virtual ~BasicParametrizedGripper();

            virtual void applyModifications(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state);

        private:

        };

    }
}


