/* 
 * File:   ParametrizedGripperLoader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 2:54 PM
 */

#pragma once

#include <models/ParametrizedGripper.hpp>
#include "BaseGripperLoader.hpp"

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class ParametrizedGripperLoader
             * @brief
             */
            class ParametrizedGripperLoader : public BaseGripperLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<ParametrizedGripperLoader> Ptr;

            public:
                ParametrizedGripperLoader();

                virtual ~ParametrizedGripperLoader();
                
                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual std::pair<std::string, boost::property_tree::ptree> write(Gripper::Ptr object);

            };

        }
    }
}


