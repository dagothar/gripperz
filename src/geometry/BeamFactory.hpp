/* 
 * File:   BeamFactory.hpp
 * Author: dagothar
 *
 * Created on October 19, 2015, 2:18 PM
 */

#pragma once

#include "Beam.hpp"

namespace gripperz {
    namespace geometry {

        /**
         * @class BeamFactory
         * @brief
         */
        class BeamFactory {
        public:
            
            static Beam::Ptr makeBeam(rw::geometry::Geometry::Ptr geometry);

        };

    }
}


