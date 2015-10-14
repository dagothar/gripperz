/* 
 * File:   GripperQualityImpl.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 11:01 AM
 */

#pragma once

#include "GripperQuality.hpp"
#include <map>

namespace gripperz {
    namespace models {

        /**
         * @class GripperQualityImpl
         * @brief
         */
        class GripperQualityImpl : public GripperQuality {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperQualityImpl> Ptr;

        public:
            GripperQualityImpl();

            virtual ~GripperQualityImpl();

            virtual GripperQuality::QualityIndexValue getIndex(const QualityIndexKey& key) const;

            virtual std::vector<QualityIndex> getIndices() const;

            virtual void setIndex(const QualityIndexKey& key, const QualityIndexValue& value);

        private:
            std::map<QualityIndexKey, QualityIndexValue> _indices;
        };

    }
}


