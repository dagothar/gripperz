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
                
                virtual Gripper::Ptr load(const std::string& filename);

                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual void save(const std::string& filename, Gripper::Ptr object);

                virtual boost::property_tree::ptree write(Gripper::Ptr object);


            private:

            };

        }
    }

}