/**
 * @file TaskGenerator.hpp
 * @brief Based on ParJawGripSampler by Jimmy
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Types.hpp"

namespace gripperz {
    namespace grasps {

        /**
         * @class GraspSource
         * @brief 
         */
        class GraspSource {
        public:
            /// Smart pointer type to this class.
            typedef rw::common::Ptr<GraspSource> Ptr;

        public:

            GraspSource() {
            }

            virtual ~GraspSource() {
            }

            virtual Grasps getGrasps() = 0;

            /**
             * @brief Returns samples (to be removed)
             * @return 
             */
            virtual Grasps getSamples() = 0;
        };

    } /* grasps */
} /* gripperz */
