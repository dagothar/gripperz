/* 
 * File:   GeometryUtils.hpp
 * Author: dagothar
 *
 * Created on October 20, 2015, 10:36 AM
 */

#pragma once

#include <rw/geometry/AABB.hpp>
#include <rw/geometry/TriMesh.hpp>
#include <rw/math/Vector3D.hpp>

namespace gripperz {
    namespace geometry {

        /**
         * @class GeometryUtils
         * @brief
         */
        class GeometryUtils {
        public:

            /**
             * Calculates AABB of the mesh.
             * @param mesh
             * @return 
             */
            template<class T = double>
            static rw::geometry::AABB<T> makeAABB(const rw::geometry::TriMesh::Ptr mesh) {
                using namespace rw::geometry;
                using namespace rw::math;

                AABB<T> aabb;

                double x0 = 0.0, x1 = 0.0, y0 = 0.0, y1 = 0.0, z0 = 0.0, z1 = 0.0;
                for (unsigned i = 0; i < mesh->getSize(); ++i) {
                    Triangle<double> tri = mesh->getTriangle(i);

                    for (unsigned j = 0; j < 3 ; ++j) {
                        Vector3D<> vj = tri[j];

                        if (vj[0] < x0) x0 = vj[0];
                        if (vj[0] > x1) x1 = vj[0];
                        if (vj[1] < y0) y0 = vj[1];
                        if (vj[1] > y1) y1 = vj[1];
                        if (vj[2] < z0) z0 = vj[2];
                        if (vj[2] > z1) z1 = vj[2];
                    }
                }

                Vector3D<> halfLengths((x1 - x0) / 2, (y1 - y0) / 2, (z1 - z0) / 2);
                aabb.setHalfLengths(halfLengths);
                
                Vector3D<> position = Vector3D<>(x0, y0, z0) + halfLengths;
                aabb.setPosition(position);

                return aabb;
            }

        };

    }
}


