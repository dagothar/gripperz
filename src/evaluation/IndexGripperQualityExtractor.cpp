/* 
 * File:   IndexGripperQualityExtractor.cpp
 * Author: dagothar
 * 
 * Created on October 14, 2015, 2:50 PM
 */

#include "IndexGripperQualityExtractor.hpp"

using namespace gripperz::evaluation;
using namespace gripperz::math;
using namespace gripperz::models;

IndexGripperQualityExtractor::IndexGripperQualityExtractor(const std::vector<models::QualityIndexKey>& indexNames) :
_indexNames(indexNames) {

}

IndexGripperQualityExtractor::~IndexGripperQualityExtractor() {
}

Vector IndexGripperQualityExtractor::extract(const models::GripperQuality::Ptr quality) const {
    unsigned n = _indexNames.size();
    Vector values(n);
    
    for (unsigned i = 0; i < n; ++i) {
        values[i] = quality->getIndex(_indexNames[i]);
    }
    
    return values;
}
