/* 
 * File:   GripperLoader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 11:44 AM
 */

#pragma once

#include <models/Gripper.hpp>
#include "GripperLoader.hpp"

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class GripperLoader
             * @brief
             */
            class BaseGripperLoader : public GripperLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<BaseGripperLoader> Ptr;

            public:
                BaseGripperLoader();

                virtual ~BaseGripperLoader();

                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual std::pair<std::string, boost::property_tree::ptree> write(Gripper::Ptr object);

            };

        }
    }

}