/**
 * @file CombineSum.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineSum.hpp"

#include <boost/foreach.hpp>


using namespace std;
using namespace gripperz::math;


CombineSum::CombineSum(const Vector& weights) :
	CombineObjectives(weights)
{}


CombineSum::~CombineSum() {}
	

Scalar CombineSum::_do_combine(const Vector& objectives) {
	Scalar q = 0.0;
	Scalar sum_w = 0.0;
	
	for (unsigned i = 0; i < objectives.size(); ++i) {
		Scalar w = getWeights()[i];
		q += w * objectives[i];
		sum_w += w;
	}
	
	return q / sum_w;
}
