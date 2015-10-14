/* 
 * File:   IndexGripperQualityExtractor.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 2:47 PM
 */

#pragma once

#include "GripperQualityExtractor.hpp"

namespace gripperz {
    namespace evaluation {

        /**
         * @class IndexGripperQualityExtractor
         * @brief Implements extractor; extracts indices according to the list of index names
         */
        class IndexGripperQualityExtractor : public GripperQualityExtractor {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<IndexGripperQualityExtractor> Ptr;

        public:
            IndexGripperQualityExtractor(const std::vector<models::QualityIndexKey>& indexNames);

            virtual ~IndexGripperQualityExtractor();

            virtual math::Vector extract(const models::GripperQuality::Ptr quality) const;

            void setIndexNames(const std::vector<models::QualityIndexKey>& _indexNames) {
                this->_indexNames = _indexNames;
            }

            std::vector<models::QualityIndexKey> getIndexNames() const {
                return _indexNames;
            }

        private:
            std::vector<models::QualityIndexKey> _indexNames;
        };

    }
}


