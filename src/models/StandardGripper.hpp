/* 
 * File:   StandardGripper.hpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 2:31 PM
 */

#pragma once

#include "BasicParametrizedGripper.hpp"
#include "ParametrizedGeometryGripper.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class StandardGripper
         * @brief
         */
        class StandardGripper : public ParametrizedGeometryGripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<StandardGripper> Ptr;
        public:
            //! Constructor.
            StandardGripper(const std::string& name);
            
            StandardGripper(const ParametrizedGeometryGripper& gripper);

            //! Destructor.
            virtual ~StandardGripper();
            
            virtual void applyModifications(rw::models::WorkCell::Ptr wc, rwsim::dynamics::DynamicWorkCell::Ptr dwc, rw::kinematics::State& state);
        };
    }
}
