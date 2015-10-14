/**
 * @file GripperQualityExtractor.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-29
 */

#include "GripperQualityExtractor.hpp"
#include "models/GripperQuality.hpp"


using namespace gripperz::evaluation;
using namespace gripperz::math;
using namespace gripperz::models;

GripperQualityExtractor::GripperQualityExtractor() {
}

GripperQualityExtractor::~GripperQualityExtractor() {
}

Vector GripperQualityExtractor::extract(GripperQuality::Ptr q) {
    Vector result(7);

    result[0] = q->getIndex("success");
    result[1] = q->getIndex("robustness");
    result[2] = q->getIndex("alignment");
    result[3] = q->getIndex("coverage");
    result[4] = q->getIndex("wrench");
    result[5] = q->getIndex("stress");
    result[6] = q->getIndex("volume");

    return result;
}

