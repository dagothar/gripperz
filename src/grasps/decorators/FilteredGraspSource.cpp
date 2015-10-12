/* 
 * File:   FilteredGraspSource.cpp
 * Author: dagothar
 * 
 * Created on October 12, 2015, 10:50 AM
 */

#include "FilteredGraspSource.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::decorators;

FilteredGraspSource::FilteredGraspSource(GraspSource::Ptr graspSource, filters::GraspFilterChain::Ptr filterChain) :
GraspSourceDecorator(graspSource),
_filterChain(filterChain) {
}

FilteredGraspSource::~FilteredGraspSource() {
}

Grasps FilteredGraspSource::getGrasps() {
    Grasps grasps = GraspSourceDecorator::getGrasps();
    grasps = _filterChain->filter(grasps);
    return grasps;
}
