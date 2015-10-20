/* 
 * File:   CostIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:26 AM
 */

#include "CostIndexCalculator.hpp"
#include <rw/geometry/GeometryUtil.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace gripperz::models;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;
using namespace rw::geometry;

CostIndexCalculator::CostIndexCalculator(double volumeLimit) :
_volumeLimit(volumeLimit) {

}

CostIndexCalculator::~CostIndexCalculator() {
}

QualityIndexValue CostIndexCalculator::calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps) {

    DEBUG << "CALCULATING VOLUME" << endl;

    double left_volume = 1e6 * GeometryUtil::estimateVolume(*gripper->getLeftFingerBody()->getGeometry()[0]->getGeometryData()->getTriMesh());
    double right_volume = 1e6 * GeometryUtil::estimateVolume(*gripper->getRightFingerBody()->getGeometry()[0]->getGeometryData()->getTriMesh());
    double finger_volume = left_volume + right_volume;

    DEBUG << "finger_volume = " << finger_volume << endl;

    double volumeIndex = 1.0 - finger_volume / _volumeLimit;

    if (volumeIndex < 0.0) {
        volumeIndex = 0.0;
    }

    return volumeIndex;
}
