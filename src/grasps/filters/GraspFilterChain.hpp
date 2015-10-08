/* 
 * File:   GraspFilterChain.hpp
 * Author: dagothar
 *
 * Created on October 8, 2015, 12:33 PM
 */

#pragma once

#include "GraspFilter.hpp"
#include <vector>

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class GraspFilterChain
             * @brief
             */
            class GraspFilterChain : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GraspFilterChain> Ptr;

            public:
                GraspFilterChain();

                virtual ~GraspFilterChain();
                
                virtual Grasps filter(Grasps grasps);
                
                void addFilter(GraspFilter::Ptr filter);
                
                void clear() {
                    _filters.clear();
                }

            private:

                std::vector<GraspFilter::Ptr> _filters;
            };

        }
    }

}
