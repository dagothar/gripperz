/* 
 * File:   ParametrizedGeometryGripper.hpp
 * Author: dagothar
 *
 * Created on October 22, 2015, 11:01 AM
 */

#pragma once

#include "ParametrizedGripper.hpp"
#include <geometry/ParametrizedMeshGenerator.hpp>

namespace gripperz {
    namespace models {

        /**
         * @class ParametrizedGeometryGripper
         * @brief
         */
        class ParametrizedGeometryGripper : public ParametrizedGripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizedGeometryGripper> Ptr;

        public:
            ParametrizedGeometryGripper(const std::string& name);

            virtual ~ParametrizedGeometryGripper();

        private:
            geometry::ParametrizedMeshGenerator::Ptr _meshGenerator;

        };

    }
}


