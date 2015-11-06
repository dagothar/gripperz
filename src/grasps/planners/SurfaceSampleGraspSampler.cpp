/* 
 * File:   SurfaceSampleGraspSampler.cpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 12:59 PM
 */

#include "SurfaceSampleGraspSampler.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace rw::math;

SurfaceSampleGraspSampler::SurfaceSampleGraspSampler(const std::vector<SurfaceSample>& ssamples) :
_ssamples(ssamples) {

}


SurfaceSampleGraspSampler::~SurfaceSampleGraspSampler() {
}

rw::math::Transform3D<> SurfaceSampleGraspSampler::sample() {
    Transform3D<> t = _idx->transform;
    
    if (++_idx == _ssamples.end()) {
        _idx = _ssamples.begin();
    }
    
    return t;
}
