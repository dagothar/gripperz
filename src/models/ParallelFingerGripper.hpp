/* 
 * File:   ParallelFingerGripper.hpp
 * Author: dagothar
 *
 * Created on October 16, 2015, 10:34 PM
 */

#pragma once

#include "WorkcellGripper.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class ParallelFingerGripper
         * @brief
         */
        class ParallelFingerGripper : public WorkcellGripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParallelFingerGripper> Ptr;

        public:
            ParallelFingerGripper(const std::string& name);

            virtual ~ParallelFingerGripper();

            virtual void initialize(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state);

        private:

        };

    }
}


