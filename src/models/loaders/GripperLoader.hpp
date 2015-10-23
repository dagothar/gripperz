/* 
 * File:   GripperLoader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 11:44 AM
 */

#pragma once

#include <rw/common/Ptr.hpp>

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class GripperLoader
             * @brief
             */
            class GripperLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GripperLoader> Ptr;

            public:
                GripperLoader();

                virtual ~GripperLoader();

            private:

            };

        }
    }

}