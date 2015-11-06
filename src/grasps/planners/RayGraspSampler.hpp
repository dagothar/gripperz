/* 
 * File:   RayGraspSampler.hpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 1:08 PM
 */

#pragma once

#include "GraspSampler.hpp"

namespace gripperz {
    namespace grasps {
        namespace planners {

            /**
             * @class RayGraspSampler
             * @brief
             */
            class RayGraspSampler : public GraspSampler {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<RayGraspSampler> Ptr;
            public:
                //! Constructor.
                RayGraspSampler();

                //! Destructor.
                virtual ~RayGraspSampler();
            };
        }
    }
}