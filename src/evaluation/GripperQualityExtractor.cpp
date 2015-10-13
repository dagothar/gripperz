/**
 * @file GripperQualityExtractor.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-29
 */

#include "GripperQualityExtractor.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::math;
using namespace gripperz::models;

GripperQualityExtractor::GripperQualityExtractor() {
}

GripperQualityExtractor::~GripperQualityExtractor() {
}

Vector GripperQualityExtractor::extract(const GripperQuality& q) {
    Vector result(7);

    result[0] = q.at("success");
    result[1] = q.at("robustness");
    result[2] = q.at("alignment");
    result[3] = q.at("coverage");
    result[4] = q.at("wrench");
    result[5] = q.at("stress");
    result[6] = q.at("volume");

    return result;
}

