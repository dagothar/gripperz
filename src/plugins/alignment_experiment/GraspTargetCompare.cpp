/* 
 * File:   GraspTargetCompare.cpp
 * Author: dagothar
 * 
 * Created on October 26, 2015, 2:45 PM
 */

#include "GraspTargetCompare.hpp"
#include <rw/math/MetricFactory.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace rwlibs::task;
using namespace rw::math;

GraspTargetCompare::GraspTargetCompare(const rw::math::Transform3D<>& origin) : _origin(origin) {

}

GraspTargetCompare::~GraspTargetCompare() {
}

bool GraspTargetCompare::operator()(rwlibs::task::GraspTarget t1, rwlibs::task::GraspTarget t2) {

    Transform3DMetric::Ptr metric = MetricFactory::makeTransform3DMetric<double>(1.0, 1.0);

    DEBUG << "g1 = " << t1.pose.P() << "dist = " << metric->distance(_origin, t1.pose) << endl;
    DEBUG << "g2 = " << t2.pose.P() << "dist = " << metric->distance(_origin, t2.pose) << endl;
    //DEBUG << ((metric->distance(_origin, t1.pose) < metric->distance(_origin, t2.pose)) ? "g1 < g2" : "g1 >= g2") << endl;

    //return metric->distance(_origin, t1.pose) < metric->distance(_origin, t2.pose);

    GraspResult::TestStatus s1 = (GraspResult::TestStatus)t1.getResult()->testStatus;
    GraspResult::TestStatus s2 = (GraspResult::TestStatus)t2.getResult()->testStatus;

    if (s1 == GraspResult::Success && s2 != GraspResult::Success) {
        return true;
    }

    if (s1 != GraspResult::Success && s2 == GraspResult::Success) {
        return false;
    }

    if (s1 == GraspResult::Interference && s2 != GraspResult::Interference) {
        return true;
    }

    if (s1 != GraspResult::Interference && s2 == GraspResult::Interference) {
        return false;
    }


    return metric->distance(_origin, t1.pose) < metric->distance(_origin, t2.pose);
}
