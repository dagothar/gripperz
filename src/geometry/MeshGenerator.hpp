/* 
 * File:   MeshGenerator.hpp
 * Author: dagothar
 *
 * Created on October 21, 2015, 3:05 PM
 */

#pragma once

#include <rw/geometry/TriMesh.hpp>

namespace gripperz {
    namespace geometry {

        /**
         * @class MeshGenerator
         * @brief Interface for classes generating mesh.
         */
        class MeshGenerator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<MeshGenerator> Ptr;

        public:
            MeshGenerator();

            virtual ~MeshGenerator();
            
            /**
             * Creates a mesh.
             * @return 
             */
            virtual rw::geometry::TriMesh::Ptr createMesh() = 0;

        private:

        };

    }
}


