/**
 * @file CombineProduct.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineProduct.hpp"

#include <cmath>


using namespace std;
using namespace gripperz::optimization;


CombineProduct::CombineProduct(const std::vector<double> weights) :
	CombineObjectives(weights)
{}


CombineProduct::~CombineProduct() {}
	

double CombineProduct::_do_combine(const std::vector<double>& objectives) {
	double q = 1.0;
	double sum_w = 0.0;
	
	for (unsigned i = 0; i < objectives.size(); ++i) {
		double w = getWeights()[i];
		q *= pow(objectives[i], w);
		sum_w += w;
	}
	
	return pow(q, 1.0/sum_w);
}
