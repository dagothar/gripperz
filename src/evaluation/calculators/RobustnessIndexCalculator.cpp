/* 
 * File:   RobustnessIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:26 AM
 */

#include "RobustnessIndexCalculator.hpp"

using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

RobustnessIndexCalculator::RobustnessIndexCalculator() {
}

RobustnessIndexCalculator::~RobustnessIndexCalculator() {
}

QualityIndex RobustnessIndexCalculator::calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {

    //TODO: implement
    
    return QualityIndex("wrench", 0.0);
}
