/* 
 * File:   StandardGripperLoader.hpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 2:38 PM
 */

#pragma once

#include "ParametrizedGeometryGripperLoader.hpp"

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class StandardGripperLoader
             * @brief
             */
            class StandardGripperLoader : public ParametrizedGeometryGripperLoader {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<StandardGripperLoader> Ptr;
            public:
                //! Constructor.
                StandardGripperLoader();

                //! Destructor.
                virtual ~StandardGripperLoader();

                virtual Gripper::Ptr read(const boost::property_tree::ptree& tree);

                virtual std::pair<std::string, boost::property_tree::ptree> write(Gripper::Ptr object);

            };
        }
    }
}