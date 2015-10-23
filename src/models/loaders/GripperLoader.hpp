/* 
 * File:   GripperLoader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 11:44 AM
 */

#pragma once

#include <models/Gripper.hpp>
#include <loaders/Loader.hpp>

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

                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual boost::property_tree::ptree write(Gripper::Ptr object);


            private:

            };

        }
    }

}