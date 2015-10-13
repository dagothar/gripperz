/**
 * @file GripperQualityExtractor.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-29
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <models/Types.hpp>
#include <math/Types.hpp>

namespace gripperz {
    namespace evaluation {

        /**
         * @class GripperQualityExtractor
         * @brief Extracts values from GripperQuality into a vector of doubles
         */
        class GripperQualityExtractor {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperQualityExtractor> Ptr;

        public:
            GripperQualityExtractor();

            virtual ~GripperQualityExtractor();

            virtual math::Vector extract(const models::GripperQuality& quality);
        };

    } /* evaluation */
} /* gripperz */
