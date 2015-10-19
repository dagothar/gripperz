/* 
 * File:   EulerBeam.cpp
 * Author: dagothar
 * 
 * Created on October 19, 2015, 2:08 PM
 */

#include "EulerBeam.hpp"

using namespace gripperz::geometry;

EulerBeam::EulerBeam(rw::geometry::Geometry::Ptr geometry) :
Beam(geometry) {

}

EulerBeam::~EulerBeam() {
}

double EulerBeam::moment(double x) {

    double M = 0;
    
    /*
     * The moment M(x) is a sum of all the forces (times distance) and the moments with position < x
     */
    BOOST_FOREACH (const Moment& moment, getMoments()) {
        if (moment.position <= x) {
            M += moment.magnitude;
        }
    }
    
    BOOST_FOREACH (const Force& force, getForces()) {
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
    return 1; // TODO: implement
}
