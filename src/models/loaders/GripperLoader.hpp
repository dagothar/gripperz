/* 
 * File:   GripperLoader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 11:26 PM
 */

#pragma once

#include <loaders/Loader.hpp>
#include <models/Gripper.hpp>

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class GripperLoader
             * @brief
             */
            class GripperLoader : public gripperz::loaders::Loader<Gripper::Ptr> {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GripperLoader> Ptr;

            public:
                GripperLoader();

                virtual ~GripperLoader();
                
                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree) = 0;

                virtual std::pair<std::string, boost::property_tree::ptree> write(Gripper::Ptr object) = 0;

            };

        }
    }
}


