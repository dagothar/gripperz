/* 
 * File:   MasterGripperLoader.hpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 1:56 PM
 */

#pragma once

#include "GripperLoader.hpp"

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class MasterGripperLoader
             * @brief
             */
            class MasterGripperLoader : public GripperLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<MasterGripperLoader> Ptr;
            public:
                //! Constructor.
                MasterGripperLoader();

                //! Destructor.
                virtual ~MasterGripperLoader();

                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual std::pair<std::string, boost::property_tree::ptree> write(Gripper::Ptr object);
            };
        }
    }
}