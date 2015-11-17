/* 
 * File:   PrototypeGripperBuilder.hpp
 * Author: dagothar
 *
 * Created on November 17, 2015, 10:32 AM
 */

#pragma once

#include "GripperBuilder.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class PrototypeGripperBuilder
         * @brief
         */
        class PrototypeGripperBuilder : public GripperBuilder {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<PrototypeGripperBuilder> Ptr;
        public:
            //! Constructor.
            PrototypeGripperBuilder();

            //! Destructor.
            virtual ~PrototypeGripperBuilder();
            
            virtual ParametrizedGripper::Ptr vectorToGripper(const math::Vector& params);

            virtual math::Vector gripperToVector(ParametrizedGripper::Ptr gripper);

        };
    }
}
