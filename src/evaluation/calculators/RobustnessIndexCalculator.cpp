/* 
 * File:   RobustnessIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:26 AM
 */

#include "RobustnessIndexCalculator.hpp"

using namespace std;
using namespace gripperz::models;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

RobustnessIndexCalculator::RobustnessIndexCalculator() {
}

RobustnessIndexCalculator::~RobustnessIndexCalculator() {
}

QualityIndexValue RobustnessIndexCalculator::calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {

    RW_WARN("Robustness is not implemented!")
    
    return 0;
}
