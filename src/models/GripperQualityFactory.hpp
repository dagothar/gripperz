/* 
 * File:   GripperQualityFactory.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 11:10 AM
 */

#pragma once

#include "GripperQuality.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class GripperQualityFactory
         * @brief
         */
        class GripperQualityFactory {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperQualityFactory> Ptr;

        public:

            static GripperQuality::Ptr makeGripperQuality();

        private:

        };

    }
}


