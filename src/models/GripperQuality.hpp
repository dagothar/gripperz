/* 
 * File:   GripperQuality.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 9:25 AM
 */

#pragma once

#include <vector>
#include <iostream>
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

            friend std::ostream& operator<<(std::ostream& stream, const GripperQuality& quality);

        private:

        };

        template<class T>
        T& operator<<(T& stream, const GripperQuality& quality) {
            std::vector<GripperQuality::QualityIndex> indices = quality.getIndices();

            stream << "GripperQuality:\n";

            for (
                    std::vector<GripperQuality::QualityIndex>::iterator i = indices.begin();
                    i != indices.end();
                    ++i
                    ) {
                stream << " - " << i->first << "= " << i->second << "\n";
            }
            stream << std::endl;

            return stream;
        }

    }
}


