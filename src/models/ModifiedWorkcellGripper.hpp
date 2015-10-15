/* 
 * File:   ModifiedWorkcellGripper.hpp
 * Author: dagothar
 *
 * Created on October 15, 2015, 6:56 PM
 */

#pragma once

#include "WorkcellGripper.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class ModifiedWorkcellGripper
         * @brief
         */
        class ModifiedWorkcellGripper : public WorkcellGripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ModifiedWorkcellGripper> Ptr;

        public:
            ModifiedWorkcellGripper(const std::string& name);

            virtual ~ModifiedWorkcellGripper();

        private:

        };

    }
}


