/* 
 * File:   ClearStatusFilter.hpp
 * Author: dagothar
 *
 * Created on October 12, 2015, 12:16 PM
 */

#pragma once

#include "GraspFilter.hpp"

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class ClearStatusFilter
             * @brief Clears grasp status from the simulation
             */
            class ClearStatusFilter : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<ClearStatusFilter> Ptr;

            public:
                ClearStatusFilter();

                virtual ~ClearStatusFilter();

                virtual Grasps filter(Grasps grasps);

            private:

            };

        }
    }
}
