/**
 * @file CombineObjectives.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineObjectives.hpp"

#include <rw/common/macros.hpp>


using namespace std;
using namespace gripperz::optimization;


CombineObjectives::CombineObjectives(const std::vector<double> weights) :
	_weights(weights)
{}


CombineObjectives::~CombineObjectives() {}
	

const std::vector<double>& CombineObjectives::getWeights() const {
	return _weights;
}


void CombineObjectives::setWeights(const std::vector<double>& weights) {
	_weights = weights;
}


double CombineObjectives::combine(const std::vector<double>& objectives) {
	if (objectives.size() != _weights.size()) {
		RW_THROW("Weights and objectives size mismatch!");
	}
	
	return _do_combine(objectives);
}
