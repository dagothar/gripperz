/* 
 * File:   OldFingerGenerator.hpp
 * Author: dagothar
 *
 * Created on October 21, 2015, 3:13 PM
 */

#pragma once

#include "ParametrizedMeshGenerator.hpp"

namespace gripperz {
    namespace geometry {

        /**
         * @class OldFingerGenerator
         * @brief
         */
        class OldFingerGenerator: public ParametrizedMeshGenerator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<OldFingerGenerator> Ptr;

        public:
            OldFingerGenerator();

            virtual ~OldFingerGenerator();
            
            virtual rw::geometry::TriMesh::Ptr createMesh();

        private:

        };

    }
}


