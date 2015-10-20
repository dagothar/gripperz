/* 
 * File:   EulerBeam.cpp
 * Author: dagothar
 * 
 * Created on October 19, 2015, 2:08 PM
 */

#include "EulerBeam.hpp"
#include <rwlibs/csg/CSGModel.hpp>
#include <rwlibs/csg/CSGModelFactory.hpp>
#include <boost/foreach.hpp>
#include <rw/geometry/GeometryUtil.hpp>
#include <geometry/GeometryUtils.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace gripperz::geometry;
using namespace rwlibs::csg;
using namespace rw::math;
using namespace rw::geometry;

EulerBeam::EulerBeam(rw::geometry::TriMesh::Ptr mesh) :
Beam(mesh) {

}

EulerBeam::~EulerBeam() {
}

double EulerBeam::moment(double x) {

    double M = 0;

    /*
     * The moment M(x) is a sum of all the forces (times distance) and the moments with position < x
     */
    BOOST_FOREACH(const Moment& moment, getMoments()) {
        if (moment.position <= x) {
            M += moment.magnitude;
        }
    }

    BOOST_FOREACH(const Force& force, getForces()) {
        if (force.position <= x) {
            double lever = x - force.position;
            M += force.magnitude * lever;
        }
    }

    return M;
}

double EulerBeam::stress(double x) {

    double hOverI = calculateHOverI(x);
    double max_stress = fabs(moment(x)) * hOverI;

    return max_stress;
}

double EulerBeam::calculateHOverI(double x) {
    const double sliceDepth = 0.001;
    const double sliceSize = 1000.0;

    AABB<double> beam_aabb = GeometryUtils::makeAABB(getBeamMesh());
    double beam_length = beam_aabb.getPosition()[0] + beam_aabb.getHalfLengths()[0];

    CSGModel::Ptr probe = CSGModelFactory::makeBox(sliceDepth, sliceSize, sliceSize);
    probe->translate(x, 0, 0);

    CSGModel::Ptr slice = new CSGModel(*getBeamMesh());
    slice->intersect(probe);

    AABB<double> slice_aabb = GeometryUtils::makeAABB(slice->getTriMesh());
    double h = slice_aabb.getHalfLengths()[2];

    double area = GeometryUtil::estimateVolume(*slice->getTriMesh()) / sliceDepth;
    if (area == 0.0) {
        return 0.0;
    }

    // adjust for beam ends
    double slice_x0 = x - sliceDepth / 2;
    double slice_x1 = x + sliceDepth / 2;
    
    double left_overlap = - slice_x0;
    double right_overlap = slice_x1 - beam_length;
//    
//    DEBUG << "slice_x0 = " << slice_x0 << endl;
//    DEBUG << "slice_x1 = " << slice_x1 << endl;
//    DEBUG << "left_overlap = " << left_overlap << endl;
//    DEBUG << "right_overlap = " << right_overlap << endl;
//    DEBUG << "area (before) = " << area << endl;
    
    if (left_overlap > 0.0) {
        area *= sliceDepth / left_overlap;
    }
    
    if (right_overlap > 0.0) {
        area *= sliceDepth / right_overlap;
    }
    
//    DEBUG << "area (after) = " << area << endl;

    Geometry::Ptr geometry = new Geometry(slice->getTriMesh());
    Vector3D<> cog = GeometryUtil::estimateCOG(*slice->getTriMesh());
    InertiaMatrix<> I = GeometryUtil::estimateInertia(area,{geometry}, Transform3D<>(-cog));

    double Iyy = I(1, 1);

    return h / Iyy;
}
