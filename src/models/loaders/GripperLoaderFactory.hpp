/* 
 * File:   GripperLoaderFactory.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 11:33 PM
 */

#pragma once

#include "GripperLoader.hpp"

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class GripperLoaderFactory
             * @brief
             */
            class GripperLoaderFactory {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GripperLoaderFactory> Ptr;

            public:
                GripperLoaderFactory();

                virtual ~GripperLoaderFactory();
                
                /**
                 * Creates gripper loader based on the gripper class name
                 * @param cls
                 * @return 
                 */
                virtual GripperLoader::Ptr getLoader(const std::string& cls);
                
                /**
                 * Creates gripper loader based on the gripper class
                 * @param gripper
                 * @return 
                 */
                virtual GripperLoader::Ptr getLoader(Gripper::Ptr gripper);
            };

        }
    }

}
