/**
 * @file CombineLog.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineLog.hpp"

#include <cmath>


using namespace std;
using namespace gripperz::math;


CombineLog::CombineLog(const Vector& weights) :
	CombineObjectives(weights)
{}


CombineLog::~CombineLog() {}
	

Scalar CombineLog::_do_combine(const Vector& objectives) {
	Scalar q = 0.0;
	Scalar sum_w = 0.0;
	
	for (unsigned i = 0; i < objectives.size(); ++i) {
		Scalar w = getWeights()[i];
		q += w * log2(objectives[i]);
		sum_w += w;
	}
	
	return pow(2.0, q / sum_w);
}
