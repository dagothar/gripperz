/* 
 * File:   GripperEvaluator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 12:55 PM
 */

#include "GripperEvaluator.hpp"

using namespace gripperz::evaluation;
using namespace gripperz::models;

GripperEvaluator::GripperEvaluator() {
}

GripperEvaluator::~GripperEvaluator() {
}

GripperQuality GripperEvaluator::evaluate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {
    GripperQuality quality;

    BOOST_FOREACH(QualityIndexCalculator::Ptr calculator, _calculators) {
        QualityIndex index = calculator->calculate(gripper, grasps);
        quality.insert(index);
    }
    
    return quality;
}

void GripperEvaluator::addQualityIndexCalculator(QualityIndexCalculator::Ptr calculator) {
    _calculators.push_back(calculator);
}
