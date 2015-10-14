/* 
 * File:   CoverageIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:30 AM
 */

#include "CoverageIndexCalculator.hpp"

using namespace gripperz::models;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

CoverageIndexCalculator::CoverageIndexCalculator() {
}

CoverageIndexCalculator::~CoverageIndexCalculator() {
}

GripperQuality::QualityIndexValue CoverageIndexCalculator::calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {
    return 0;
}
