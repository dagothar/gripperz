/* 
 * File:   GraspFilter.hpp
 * Author: dagothar
 *
 * Created on October 8, 2015, 12:17 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <grasps/Types.hpp>

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class GraspFilter
             * @brief Interface for grasp filters.
             */
            class GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GraspFilter> Ptr;

            public:
                GraspFilter() {}

                virtual ~GraspFilter() {}
                
                virtual Grasps filter(Grasps grasps) = 0;

            };

        }
    }
}


