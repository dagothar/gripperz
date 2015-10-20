/* 
 * File:   StressIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:26 AM
 */

#include "StressIndexCalculator.hpp"
#include "geometry/Beam.hpp"
#include "geometry/BeamFactory.hpp"
#include <rw/geometry/TriMesh.hpp>
#include <geometry/GeometryUtils.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace rw::geometry;
using namespace gripperz::models;
using namespace gripperz::geometry;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

StressIndexCalculator::StressIndexCalculator(double stressLimit) :
_stressLimit(stressLimit) {

}

StressIndexCalculator::~StressIndexCalculator() {
}

QualityIndexValue StressIndexCalculator::calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps) {
    DEBUG << "CALCULATING STRESS" << endl;
    
    const double Resolution = 0.001;
    const double MPa = 1000000;

    /* 1. get finger geometry */
    TriMesh::Ptr left_mesh = gripper->getLeftFingerBody()->getGeometry()[0]->getGeometryData()->getTriMesh();
    TriMesh::Ptr right_mesh = gripper->getRightFingerBody()->getGeometry()[0]->getGeometryData()->getTriMesh();

    /* 2. find finger dimensions */
    AABB<double> left_aabb = GeometryUtils::makeAABB(left_mesh);
    AABB<double> right_aabb = GeometryUtils::makeAABB(right_mesh);
    double left_length = left_aabb.getPosition()[0] + left_aabb.getHalfLengths()[0];
    double right_length = right_aabb.getPosition()[0] + right_aabb.getHalfLengths()[0];

    /* 3. create beams and apply forces */
    double force = gripper->getDynamicDevice()->getMotorForceLimits()[0];

    Beam::Ptr left_beam = BeamFactory::makeBeam(left_mesh);
    Beam::Ptr right_beam = BeamFactory::makeBeam(right_mesh);

    left_beam->addMoment(-force * left_length, 0.0);
    left_beam->addForce(force, 0.0);
    left_beam->addForce(-force, left_length);

    right_beam->addMoment(-force * right_length, 0.0);
    right_beam->addForce(force, 0.0);
    right_beam->addForce(-force, right_length);

    /* 4. find the largest stress */
    double max_stress = 0.0;
    for (double x = 0.0; x <= left_length; x += Resolution) {
        double stress = left_beam->stress(x);

        if (stress > max_stress) {
            max_stress = stress;
        }
    }

    for (double x = 0.0; x <= right_length; x += Resolution) {
        double stress = right_beam->stress(x);

        if (stress > max_stress) {
            max_stress = stress;
        }
    }
    
    max_stress /= MPa;
    
    DEBUG << "max_stress = " << max_stress << endl;

    /* 5. calculate index */
    double stress_index = 1.0 - (max_stress / _stressLimit);

    if (stress_index < 0.0) {
        stress_index = 0.0;
    }

    return stress_index;
}
