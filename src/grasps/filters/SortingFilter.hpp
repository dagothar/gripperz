/* 
 * File:   SortingFilter.hpp
 * Author: dagothar
 *
 * Created on October 26, 2015, 1:55 PM
 */

#pragma once

#include "GraspFilter.hpp"
#include <algorithm>

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class SortingFilter
             * @brief Sorts grasps according to some operator COMPARE
             */
            class SortingFilter : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<SortingFilter> Ptr;

                //! Function object that compares GraspTargets

                class Compare {
                public:
                    //! Smart pointer.
                    typedef rw::common::Ptr<Compare> Ptr;
                    
                public:

                    virtual bool operator()(rwlibs::task::GraspTarget t1, rwlibs::task::GraspTarget t2) = 0;
                };

            public:

                SortingFilter(Compare::Ptr comp);

                virtual ~SortingFilter();

                virtual Grasps filter(Grasps grasps);

            private:
                Compare::Ptr _comp;
            };

        }
    }

}