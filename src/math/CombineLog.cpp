/**
 * @file CombineLog.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineLog.hpp"

#include <cmath>


using namespace std;
using namespace gripperz::math;


CombineLog::CombineLog(const std::vector<double> weights) :
	CombineObjectives(weights)
{}


CombineLog::~CombineLog() {}
	

double CombineLog::_do_combine(const std::vector<double>& objectives) {
	double q = 0.0;
	double sum_w = 0.0;
	
	for (unsigned i = 0; i < objectives.size(); ++i) {
		double w = getWeights()[i];
		q += w * log2(objectives[i]);
		sum_w += w;
	}
	
	return pow(2.0, q / sum_w);
}
