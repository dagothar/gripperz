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

    double I = calculateSecondMomentOfAreaAt(x);

    double sigma = moment(x) / I;

    return sigma;
}

double EulerBeam::calculateSecondMomentOfAreaAt(double x) {
    const double sliceDepth = 0.001;
    const double sliceSize = 1000.0;

    CSGModel::Ptr probe = CSGModelFactory::makeBox(sliceSize, sliceSize, sliceDepth);
    probe->translate(0, 0, x);

    CSGModel::Ptr slice = new CSGModel(*getBeamMesh());
    slice->intersect(probe);

    double area = GeometryUtil::estimateVolume(*slice->getTriMesh()) / sliceDepth;
    Geometry::Ptr geometry = new Geometry(slice->getTriMesh());
    Vector3D<> cog = GeometryUtil::estimateCOG(*slice->getTriMesh());
    InertiaMatrix<> I = GeometryUtil::estimateInertia(area,{geometry}, Transform3D<>(-cog));

    double Iyy = I(1, 1);

    return Iyy;
}
