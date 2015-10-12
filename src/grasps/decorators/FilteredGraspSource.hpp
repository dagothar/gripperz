/* 
 * File:   FilteredGraspSource.hpp
 * Author: dagothar
 *
 * Created on October 12, 2015, 10:47 AM
 */

#pragma once

#include "GraspSourceDecorator.hpp"
#include <grasps/filters/GraspFilterChain.hpp>

namespace gripperz {
    namespace grasps {
        namespace decorators {

            /**
             * @class FilteredGraspSource
             * @brief A grasp source decorator which incorporates a filter chain
             * that filters the outgoing grasps.
             */
            class FilteredGraspSource : public GraspSourceDecorator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<FilteredGraspSource> Ptr;

            public:
                FilteredGraspSource(GraspSource::Ptr graspSource, filters::GraspFilterChain::Ptr filterChain);

                virtual ~FilteredGraspSource();
                
                virtual Grasps getGrasps();

                void setFilterChain(filters::GraspFilterChain::Ptr _filterChain) {
                    this->_filterChain = _filterChain;
                }

                filters::GraspFilterChain::Ptr getFilterChain() const {
                    return _filterChain;
                }

            private:
                filters::GraspFilterChain::Ptr _filterChain;
            };

        }
    }

}