/* 
 * File:   GraspSampler.hpp
 * Author: dagothar
 *
 * Created on November 5, 2015, 1:23 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>

namespace gripperz {
    namespace grasps {
        namespace planners {

            /**
             * @class GraspSampler
             * @brief Interface for samplers generating grasp candidates
             */
            class GraspSampler {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GraspSampler> Ptr;
            public:
                //! Constructor.
                GraspSampler();

                //! Destructor.
                virtual ~GraspSampler();
                
                /**
                 * Generates a candidate grasp.
                 * @return 
                 */
                virtual rw::math::Transform3D<> sample() = 0;
            };
        }
    }
}