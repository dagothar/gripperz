/* 
 * File:   WorkcellGripper.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 10:58 PM
 */

#pragma once

#include "Gripper.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class WorkcellGripper
         * @brief A gripper pre-existing in the workcell
         */
        class WorkcellGripper : public Gripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<WorkcellGripper> Ptr;

        public:
            WorkcellGripper(const std::string& name);

            virtual ~WorkcellGripper();

        private:
            std::string _dynamicDeviceName;
            //rw::models::Device::Ptr _gripperDevice;
            

        };

    }
}


