/**
 * @file SurfaceSample.hpp
 * @author Adam Wolniakowski
 * @date 03-07-2015
 */

#pragma once

#include <string>
#include <vector>
#include <rw/math/Transform3D.hpp>

namespace gripperz {
    namespace grasps {

        class SurfaceSample {
        public:
            /// Constructor

            SurfaceSample(rw::math::Transform3D<> transform = rw::math::Transform3D<>(), double graspW = 0.0) :
            transform(transform),
            graspW(graspW) {
            }

            /// Destructor

            ~SurfaceSample() {
            }

            /**
             * @brief Loads a vector of SurfaceSample from XML file
             */
            static std::vector<SurfaceSample> loadFromXML(const std::string& filename);

            /**
             * @brief Saves a vector of SurfaceSample to XML file
             */
            static void saveToXML(const std::string& filename, const std::vector<SurfaceSample>& samples);

        public:
            rw::math::Transform3D<> transform;
            double graspW;
        };

    } /* grasps */
} /* gripperz */
