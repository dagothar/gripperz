/* 
 * File:   GripperQualityImpl.cpp
 * Author: dagothar
 * 
 * Created on October 14, 2015, 11:01 AM
 */

#include <map>

#include "GripperQualityImpl.hpp"

using namespace std;
using namespace gripperz::models;

GripperQualityImpl::GripperQualityImpl() {
}

GripperQualityImpl::~GripperQualityImpl() {
}

GripperQuality::QualityIndexValue GripperQualityImpl::getIndex(const QualityIndexKey& key) const {
    return _indices.at(key);
}

vector<GripperQuality::QualityIndex> GripperQualityImpl::getIndices() const {

    vector<QualityIndex> indices;

    for (
            map<QualityIndexKey, QualityIndexValue>::const_iterator i = _indices.begin();
            i != _indices.end();
            ++i
            ) {
        indices.push_back(QualityIndex(i->first, i->second));
    }

    return indices;

}

void GripperQualityImpl::setIndex(const QualityIndexKey& key, const QualityIndexValue& value) {
    _indices[key] = value;
}
