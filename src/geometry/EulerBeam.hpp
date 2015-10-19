/* 
 * File:   EulerBeam.hpp
 * Author: dagothar
 *
 * Created on October 19, 2015, 2:07 PM
 */

#pragma once

#include "Beam.hpp"

namespace gripperz {
    namespace geometry {

        /**
         * @class EulerBeam
         * @brief
         */
        class EulerBeam : public Beam {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<EulerBeam> Ptr;

        public:
            EulerBeam(rw::geometry::TriMesh::Ptr mesh);

            virtual ~EulerBeam();

            virtual double moment(double x);

            virtual double stress(double x);

        protected:
            virtual double calculateSecondMomentOfAreaAt(double x);

        private:

        };

    }
}


