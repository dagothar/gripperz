/* 
 * File:   GripperQualityLoader.hpp
 * Author: dagothar
 *
 * Created on October 23, 2015, 1:06 PM
 */

#pragma once

#include <loaders/Loader.hpp>
#include <models/GripperQuality.hpp>

namespace gripperz {
    namespace models {
        namespace loaders {

            /**
             * @class GripperQualityLoader
             * @brief
             */
            class GripperQualityLoader : public gripperz::loaders::Loader<GripperQuality::Ptr> {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<GripperQualityLoader> Ptr;

            public:
                GripperQualityLoader();

                virtual ~GripperQualityLoader();

                virtual GripperQuality::Ptr read(const boost::property_tree::ptree& tree);

                virtual boost::property_tree::ptree write(GripperQuality::Ptr object);

            private:

            };

        }
    }
}

