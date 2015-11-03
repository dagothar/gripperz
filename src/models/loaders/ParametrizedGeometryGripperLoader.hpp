/* 
 * File:   ParametrizedGeometryGripperLoader.hpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 12:42 PM
 */

#pragma once

#include "ParametrizedGripperLoader.hpp"

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class ParametrizedGeometryGripperLoader
             * @brief
             */
            class ParametrizedGeometryGripperLoader: public ParametrizedGripperLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<ParametrizedGeometryGripperLoader> Ptr;
            public:
                //! Constructor.
                ParametrizedGeometryGripperLoader();

                //! Destructor.
                virtual ~ParametrizedGeometryGripperLoader();
                
                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual std::pair<std::string, boost::property_tree::ptree> write(Gripper::Ptr object);
            };
        }
    }
}