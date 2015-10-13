/* 
 * File:   CostIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:26 AM
 */

#include "CostIndexCalculator.hpp"

using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

CostIndexCalculator::CostIndexCalculator() {
}

CostIndexCalculator::~CostIndexCalculator() {
}

QualityIndex CostIndexCalculator::calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {

    //TODO: implement
    
    return QualityIndex("cost", 0.0);
}
