/* 
 * File:   ParametrizedMeshGenerator.hpp
 * Author: dagothar
 *
 * Created on October 21, 2015, 3:09 PM
 */

#pragma once

#include "MeshGenerator.hpp"
#include <parametrization/Parametrized.hpp>

namespace gripperz {
    namespace geometry {

        /**
         * @class ParametrizedMeshGenerator
         * @brief
         */
        class ParametrizedMeshGenerator : public MeshGenerator, public parametrization::Parametrized {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizedMeshGenerator> Ptr;

        public:
            ParametrizedMeshGenerator(parametrization::Parametrization::Ptr parametrization =
                    rw::common::ownedPtr(new parametrization::Parametrization()));

            virtual ~ParametrizedMeshGenerator();
        };

    }
}


