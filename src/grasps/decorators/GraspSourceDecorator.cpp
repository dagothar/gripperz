/* 
 * File:   GraspSourceDecorator.cpp
 * Author: dagothar
 * 
 * Created on October 11, 2015, 8:19 PM
 */

#include "GraspSourceDecorator.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::decorators;

GraspSourceDecorator::GraspSourceDecorator(GraspSource::Ptr graspSource) :
_graspSource(graspSource) {
}

GraspSourceDecorator::~GraspSourceDecorator() {
}

Grasps GraspSourceDecorator::getGrasps() {
    return _graspSource->getGrasps();
}
