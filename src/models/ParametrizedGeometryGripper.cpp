/* 
 * File:   ParametrizedGeometryGripper.cpp
 * Author: dagothar
 * 
 * Created on October 22, 2015, 11:10 AM
 */

#include "ParametrizedGeometryGripper.hpp"
#include <rw/models/RigidObject.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace gripperz::models;
using namespace gripperz::geometry;
using namespace rw::models;
using namespace rwsim::dynamics;
using namespace rw::math;
using namespace rw::kinematics;
using namespace rw::models;
using namespace rw::geometry;
using namespace rw::common;
using namespace rw::graphics;
using namespace std;

ParametrizedGeometryGripper::ParametrizedGeometryGripper(const std::string& name) :
ParametrizedGripper(name) {
}

ParametrizedGeometryGripper::ParametrizedGeometryGripper(const ParametrizedGripper& gripper) :
ParametrizedGripper(gripper) {
}

ParametrizedGeometryGripper::~ParametrizedGeometryGripper() {
}

void ParametrizedGeometryGripper::applyModifications(WorkCell::Ptr wc, DynamicWorkCell::Ptr dwc, State& state) {
    INFO << "Modifying gripper geometry" << endl;
    
    /* remove old objects */
    wc->removeObject(getLeftFingerObject().get());
    wc->removeObject(getRightFingerObject().get());
    
    /* create finger geometries */
    _meshGenerator->setParametrization(getParametrization());
    TriMesh::Ptr left_mesh = _meshGenerator->createMesh();
    TriMesh::Ptr right_mesh = _meshGenerator->createMesh();
    Geometry::Ptr left_geo = ownedPtr(new Geometry(left_mesh, "leftFingerGeo"));
    Geometry::Ptr right_geo = ownedPtr(new Geometry(right_mesh, "rightFingerGeo"));
    left_geo->setTransform(Transform3D<>());
    right_geo->setTransform(Transform3D<>(Vector3D<>(), Rotation3D<>(1, 0, 0, 0, 1, 0, 0, 0, -1)));
    
    /* create finger models */
    Model3D::Ptr left_model = ownedPtr(new Model3D("leftModel"));
    left_model->addTriMesh(Model3D::Material("stlmat", 0.6f, 0.6f, 0.6f), *left_mesh);
    left_model->setTransform(Transform3D<>());
    Model3D::Ptr right_model = ownedPtr(new Model3D("rightModel"));
    right_model->addTriMesh(Model3D::Material("stlmat", 0.6f, 0.6f, 0.6f), *right_mesh);
    right_model->setTransform(Transform3D<>(Vector3D<>(), Rotation3D<>(1, 0, 0, 0, 1, 0, 0, 0, -1)));
    
    /* create finger objects */
    RigidObject::Ptr left_obj = ownedPtr(new RigidObject(getLeftFingerObject()->getBase()));
    left_obj->addGeometry(left_geo);
    left_obj->addModel(left_model);
    RigidObject::Ptr right_obj = ownedPtr(new RigidObject(getRightFingerObject()->getBase()));
    right_obj->addGeometry(right_geo);
    right_obj->addModel(right_model);
    
    /* add objects to wc & dwc */
    wc->add(left_obj);
    getLeftFingerBody()->setObject(left_obj);
    wc->add(right_obj);
    getRightFingerBody()->setObject(right_obj);
}
