/* 
 * File:   GripperEvaluator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 12:55 PM
 */

#include "GripperEvaluator.hpp"
#include "models/GripperQualityFactory.hpp"

using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::models;

GripperEvaluator::GripperEvaluator() {
}

GripperEvaluator::~GripperEvaluator() {
}

GripperQuality::Ptr GripperEvaluator::evaluate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {
    GripperQuality::Ptr quality = GripperQualityFactory::makeGripperQuality();

    BOOST_FOREACH(KeyCalculator& keyCalculator, _calculators) {
        QualityIndexValue value = keyCalculator.second->calculate(gripper, grasps);
        quality->setIndex(keyCalculator.first, value);
    }

    return quality;
}

void GripperEvaluator::addQualityIndexCalculator(const models::QualityIndexKey& indexName, QualityIndexCalculator::Ptr calculator) {
    _calculators.push_back(make_pair(indexName, calculator));
}

vector<QualityIndexKey> GripperEvaluator::getIndexKeys() const {
    vector<QualityIndexKey> keys;
    
    BOOST_FOREACH(const KeyCalculator& keyCalculator, _calculators) {
        keys.push_back(keyCalculator.first);
    }
    
    return keys;
}
