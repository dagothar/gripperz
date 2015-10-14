/* 
 * File:   GripperQuality.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 9:25 AM
 */

#pragma once

#include <vector>
#include <rw/common/Ptr.hpp>

namespace gripperz {
    namespace models {

        /**
         * @class GripperQuality
         * @brief Interface for storing gripper qualities
         */
        class GripperQuality {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperQuality> Ptr;

            //! Defines the type for the quality index key
            typedef std::string QualityIndexKey;

            //! Defines the type for the quality index value
            typedef double QualityIndexValue;

            //! Defines the quality index type
            typedef std::pair<QualityIndexKey, QualityIndexValue> QualityIndex;

        public:
            GripperQuality();

            virtual ~GripperQuality();

            virtual QualityIndexValue getIndex(const QualityIndexKey& key) const = 0;

            virtual void setIndex(const QualityIndexKey& key, const QualityIndexValue& value) = 0;

            virtual std::vector<QualityIndex> getIndices() const = 0;

        private:

        };

    }
}


