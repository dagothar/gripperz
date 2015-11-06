/* 
 * File:   ParallelGripperGraspPlanner.hpp
 * Author: dagothar
 *
 * Created on November 2, 2015, 9:31 AM
 */

#pragma once

#include "GraspPlanner.hpp"
#include <rw/models/Object.hpp>
#include <rw/kinematics/State.hpp>
#include <models/Gripper.hpp>

namespace gripperz {
    namespace grasps {
        namespace planners {

            /**
             * @class ParallelGripperGraspPlanner
             * @brief
             */
            class ParallelGripperGraspPlanner : public GraspPlanner {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<ParallelGripperGraspPlanner> Ptr;
            public:
                //! Constructor.
                ParallelGripperGraspPlanner(unsigned nGrasps);

                //! Destructor.
                virtual ~ParallelGripperGraspPlanner();

                virtual Grasps planGrasps(unsigned nGrasps);
                
            protected:
                
            private:
                rw::models::Object::Ptr _targetObject;
                gripperz::models::Gripper::Ptr _gripper;
                rw::kinematics::State _state;
                
                rw::math::Transform3D<> _approach;
                rw::math::Transform3D<> _retract;
                
            };
        }
    }
}
