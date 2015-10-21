/* 
 * File:   OldFingerGenerator.cpp
 * Author: dagothar
 * 
 * Created on October 21, 2015, 3:14 PM
 */

#include "OldFingerGenerator.hpp"
#include "JawPrimitive.hpp"

using namespace rw::common;
using namespace rw::math;
using namespace rw::geometry;
using namespace gripperz::geometry;
using namespace gripperz::parametrization;

const Parametrization::ParameterList PARAMETERS{
    {"length", 0.1},
    {"width", 0.025},
    {"depth", 0.02},
    {"chfdepth", 0},
    {"chfangle", 45},
    {"cutpos", 0.075},
    {"cutdepth", 0},
    {"cutangle", 90},
    {"cuttilt", 0}
};

OldFingerGenerator::OldFingerGenerator() :
ParametrizedMeshGenerator(ownedPtr(new Parametrization(PARAMETERS))) {
}

OldFingerGenerator::~OldFingerGenerator() {
}

TriMesh::Ptr OldFingerGenerator::createMesh() {
    Q q(11);

    q(0) = 0;
    q(1) = getParameter("length");
    q(2) = getParameter("width");
    q(3) = getParameter("depth");
    q(4) = getParameter("chfdepth");
    q(5) = getParameter("chfangle") * Deg2Rad;
    q(6) = getParameter("length") - getParameter("cutpos");
    q(7) = getParameter("cutdepth");
    q(8) = getParameter("cutangle") * Deg2Rad;
    q(9) = 0.0;
    q(10) = getParameter("cuttilt") * Deg2Rad;

    JawPrimitive::Ptr jaw = ownedPtr(new JawPrimitive(q));
    TriMesh::Ptr mesh = jaw->createMesh();
    
    return mesh;
}
