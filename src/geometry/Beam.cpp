/* 
 * File:   Beam.cpp
 * Author: dagothar
 * 
 * Created on October 19, 2015, 1:53 PM
 */

#include "Beam.hpp"

using namespace gripperz::geometry;

Beam::Beam(rw::geometry::TriMesh::Ptr mesh) :
_beamMesh(mesh) {
}

Beam::~Beam() {
}

