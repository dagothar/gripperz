/* 
 * File:   GripperFactory.hpp
 * Author: dagothar
 *
 * Created on October 16, 2015, 10:58 PM
 */

#pragma once

#include "WorkcellGripper.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class GripperFactory
         * @brief
         */
        class GripperFactory {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperFactory> Ptr;

        public:
            GripperFactory();

            virtual ~GripperFactory();

            virtual WorkcellGripper::Ptr makeGripper(const std::string& classId, const std::string& name);

        private:

        };

    }
}


