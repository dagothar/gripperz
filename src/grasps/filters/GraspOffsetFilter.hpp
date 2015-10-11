/* 
 * File:   OffsetGraspFilter.hpp
 * Author: dagothar
 *
 * Created on October 11, 2015, 10:30 PM
 */

#pragma once

#include "GraspFilter.hpp"

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class OffsetGraspFilter
             * @brief
             */
            class GraspOffsetFilter : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GraspOffsetFilter> Ptr;

            public:
                GraspOffsetFilter(const rw::math::Transform3D<>& offset);

                virtual ~GraspOffsetFilter();
                
                virtual Grasps filter(Grasps grasps);

                void setOffset(const rw::math::Transform3D<>& _offset) {
                    this->_offset = _offset;
                }

                rw::math::Transform3D<> getOffset() const {
                    return _offset;
                }

            private:
                rw::math::Transform3D<> _offset;
            };

        }
    }

}