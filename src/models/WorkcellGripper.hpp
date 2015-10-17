/* 
 * File:   WorkcellGripper.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 10:58 PM
 */

#pragma once

#include "Gripper.hpp"
#include <rw/models/WorkCell.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>

namespace gripperz {
    namespace models {

        /**
         * @class WorkcellGripper
         * @brief A basic gripper pre-existing in the workcell with RobWork implementation 
         */
        class WorkcellGripper : public Gripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<WorkcellGripper> Ptr;

        public:
            WorkcellGripper(const std::string& name);

            virtual ~WorkcellGripper();

            

        private:
           
        };

    }
}


