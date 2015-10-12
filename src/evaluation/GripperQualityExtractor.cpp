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


Vector GripperQualityExtractor::extract(OldGripperQuality::Ptr q) {
	Vector result(7);
	
	result[0] = q->success;
	result[1] = q->robustness;
	result[2] = q->alignment;
	result[3] = q->coverage;
	result[4] = q->wrench;
	result[5] = q->stress;
	result[6] = q->volume;
	
	return result;
}
