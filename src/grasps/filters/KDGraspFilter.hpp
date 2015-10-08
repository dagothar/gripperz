/* 
 * File:   KDGraspFilter.
 * Author: dagothar
 *
 * Created on October 8, 2015, 2:56 PM
 */

#pragma once

#include "GraspFilter.hpp"

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class KDGraspFilter
             * @brief
             */
            class KDGraspFilter : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<KDGraspFilter> Ptr;

            public:
                KDGraspFilter(const rw::math::Q& filteringBox);

                virtual ~KDGraspFilter();

                virtual Grasps filter(Grasps grasps);

                rw::math::Q getFilteringBox() const {
                    return _filteringBox;
                }

                void setFilteringBox(const rw::math::Q& _filteringBox) {
                    this->_filteringBox = _filteringBox;
                }


            private:
                rw::math::Q _filteringBox;
            };

        }
    }

}
