/* 
 * File:   GripperQualityFactory.cpp
 * Author: dagothar
 * 
 * Created on October 14, 2015, 11:12 AM
 */

#include "GripperQualityFactory.hpp"
#include "GripperQualityImpl.hpp"

using namespace gripperz::models;
using namespace rw::common;

GripperQuality::Ptr GripperQualityFactory::makeGripperQuality() {
    GripperQuality::Ptr gripperQuality = ownedPtr(new GripperQualityImpl());

    return gripperQuality;
}
