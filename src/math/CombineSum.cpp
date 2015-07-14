/**
 * @file CombineSum.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineSum.hpp"

#include <boost/foreach.hpp>


using namespace std;
using namespace gripperz::math;


CombineSum::CombineSum(const std::vector<double> weights) :
	CombineObjectives(weights)
{}


CombineSum::~CombineSum() {}
	

double CombineSum::_do_combine(const std::vector<double>& objectives) {
	double q = 0.0;
	double sum_w = 0.0;
	
	for (unsigned i = 0; i < objectives.size(); ++i) {
		double w = getWeights()[i];
		q += w * objectives[i];
		sum_w += w;
	}
	
	return q / sum_w;
}
