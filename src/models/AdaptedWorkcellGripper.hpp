/* 
 * File:   AdaptedWorkcellGripper.hpp
 * Author: dagothar
 *
 * Created on October 12, 2015, 11:18 PM
 */

#pragma once

#include "Gripper.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class AdaptedWorkcellGripper
         * @brief Interface for grippers which alter the workcell & DWC
         */
        class AdaptedWorkcellGripper : public Gripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<AdaptedWorkcellGripper> Ptr;

        public:
            AdaptedWorkcellGripper();

            virtual ~AdaptedWorkcellGripper();
            
            virtual void apply(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell dwc) = 0;

        private:

        };

    }
}


