/* 
 * File:   ParametrizedMeshGeneratorLoader.hpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 1:00 PM
 */

#pragma once

#include <loaders/Loader.hpp>
#include <geometry/ParametrizedMeshGenerator.hpp>

namespace gripperz {
    namespace geometry {
        namespace loaders {

            /**
             * @class ParametrizedMeshGeneratorLoader
             * @brief
             */
            class ParametrizedMeshGeneratorLoader : public gripperz::loaders::Loader<ParametrizedMeshGenerator::Ptr> {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<ParametrizedMeshGeneratorLoader> Ptr;
            public:
                //! Constructor.
                ParametrizedMeshGeneratorLoader();

                //! Destructor.
                virtual ~ParametrizedMeshGeneratorLoader();

                virtual ParametrizedMeshGenerator::Ptr read(const boost::property_tree::ptree& tree);

                virtual std::pair<std::string, boost::property_tree::ptree> write(ParametrizedMeshGenerator::Ptr object);
            };
        }
    }
}