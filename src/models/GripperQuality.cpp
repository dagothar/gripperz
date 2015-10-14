/* 
 * File:   GripperQuality.cpp
 * Author: dagothar
 * 
 * Created on October 14, 2015, 9:28 AM
 */

#include <boost/foreach.hpp>

#include "GripperQuality.hpp"

using namespace std;
using namespace gripperz::models;

GripperQuality::GripperQuality() {
}

GripperQuality::~GripperQuality() {
}

//ostream& gripperz::models::operator<<(ostream& stream, const GripperQuality& quality) {
//
//    vector<GripperQuality::QualityIndex> indices = quality.getIndices();
//    
//    stream << "GripperQuality:\n";
//    BOOST_FOREACH(GripperQuality::QualityIndex& index, indices) {
//        stream << " - " << index.first << "= " << index.second << "\n";
//    }
//    stream << endl;
//    
//    return stream;
//}