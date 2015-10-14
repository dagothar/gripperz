/* 
 * File:   AlignmentIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:33 AM
 */

#include "AlignmentIndexCalculator.hpp"

using namespace gripperz::models;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

AlignmentIndexCalculator::AlignmentIndexCalculator() {
}

AlignmentIndexCalculator::~AlignmentIndexCalculator() {
}

GripperQuality::QualityIndexValue AlignmentIndexCalculator::calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {
    return 0;
}
