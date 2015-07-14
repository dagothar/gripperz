/**
 * @file CombineProduct.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineProduct.hpp"

#include <cmath>


using namespace std;
using namespace gripperz::math;


CombineProduct::CombineProduct(const Vector& weights) :
	CombineObjectives(weights)
{}


CombineProduct::~CombineProduct() {}
	

Scalar CombineProduct::_do_combine(const Vector& objectives) {
	Scalar q = 1.0;
	Scalar sum_w = 0.0;
	
	for (unsigned i = 0; i < objectives.size(); ++i) {
		Scalar w = getWeights()[i];
		q *= pow(objectives[i], w);
		sum_w += w;
	}
	
	return pow(q, 1.0/sum_w);
}
