/* 
 * File:   BasicParametrizedGripperLoader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 2:54 PM
 */

#pragma once

#include <models/ParametrizedGripper.hpp>
#include "ParametrizedGripperLoader.hpp"

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class BasicParametrizedGripperLoader
             * @brief
             */
            class BasicParametrizedGripperLoader : public ParametrizedGripperLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<BasicParametrizedGripperLoader> Ptr;

            public:
                BasicParametrizedGripperLoader();

                virtual ~BasicParametrizedGripperLoader();
                
                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual std::pair<std::string, boost::property_tree::ptree> write(Gripper::Ptr object);
            };

        }
    }
}


