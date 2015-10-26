/* 
 * File:   SimpleAlignmentMetric.hpp
 * Author: dagothar
 *
 * Created on October 24, 2015, 6:58 PM
 */

#pragma once

#include <rw/math/Metric.hpp>
#include <rw/math/Transform3D.hpp>
#include <iostream>

/**
 * @class SimpleAlignmentMetric
 * @brief
 */
template <class T>
class SimpleAlignmentMetric : public rw::math::Metric<rw::math::Transform3D<T> > {
public:

    SimpleAlignmentMetric(const rw::math::Vector3D<T>& axis) :
    _axis(axis) {
    }

protected:

    T doDistance(const rw::math::Transform3D<T>& t) const {
        rw::math::Vector3D<T> r = t.R() * _axis;
        double a = rw::math::angle(r, _axis) * rw::math::Rad2Deg;
        
        rw::math::Vector3D<> xy(t.P()[0], 0, t.P()[2]);
        double p = xy.norm2() * 1000.0;
        
        rw::common::Log::infoLog() << "A = " << a << " P = " << p << std::endl;
        
        double dist = (a > p) ? a : p;
        
        return dist;
    }

    T doDistance(const rw::math::Transform3D<T>& a, const rw::math::Transform3D<T>& b) const {
        return doDistance(inverse(b) * a);
    }
    
private:
    rw::math::Vector3D<T> _axis;
};



