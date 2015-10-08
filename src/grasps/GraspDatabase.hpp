/**
 * @file GraspDatabase.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-07
 */

#pragma once

#include "GraspSource.hpp"

namespace gripperz {
    namespace grasps {

        /**
         * @class GraspDatabase
         * @brief A source of grasps for simulating gripper performance.
         */
        class GraspDatabase : public GraspSource {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GraspDatabase> Ptr;

        public:
            GraspDatabase() {}
            
            virtual ~GraspDatabase() {}

            virtual Grasps getGrasps() = 0;

            //! Temporary.
            virtual Grasps getSamples() = 0;


        };

    } /* grasps */
} /* gripperz */
