/* 
 * File:   ParametrizedGripper.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 8:44 AM
 */

#pragma once

#include "Gripper.hpp"
#include <parametrization/Parametrized.hpp>

namespace gripperz {
    namespace models {

        /**
         * @class ParametrizedGripper
         * @brief
         */
        class ParametrizedGripper : public Gripper, public parametrization::Parametrized {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizedGripper> Ptr;

        public:
            ParametrizedGripper(
                    const std::string& name,
                    parametrization::Parametrization::Ptr parametrization = rw::common::ownedPtr(new parametrization::Parametrization())
                    );
            
            ParametrizedGripper(
                    const Gripper& gripper,
                    parametrization::Parametrization::Ptr parametrization = rw::common::ownedPtr(new parametrization::Parametrization())
                    );

            virtual ~ParametrizedGripper();
            
            virtual ParametrizedGripper* clone() const;
        };

    }
}


