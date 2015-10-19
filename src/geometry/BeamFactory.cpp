/* 
 * File:   BeamFactory.cpp
 * Author: dagothar
 * 
 * Created on October 19, 2015, 2:20 PM
 */

#include "BeamFactory.hpp"
#include "EulerBeam.hpp"

using namespace gripperz::geometry;
using namespace rw::common;

Beam::Ptr BeamFactory::makeBeam(rw::geometry::TriMesh::Ptr mesh) {
    return ownedPtr(new EulerBeam(mesh));
}
