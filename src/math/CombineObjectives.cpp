/**
 * @file CombineObjectives.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineObjectives.hpp"

#include <rw/common/macros.hpp>


using namespace std;
using namespace gripperz::math;


CombineObjectives::CombineObjectives(const Vector& weights) :
	_weights(weights)
{}


CombineObjectives::~CombineObjectives() {}
	

const Vector& CombineObjectives::getWeights() const {
	return _weights;
}


void CombineObjectives::setWeights(const Vector& weights) {
	_weights = weights;
}


Scalar CombineObjectives::combine(const Vector& objectives) {
	if (objectives.size() != _weights.size()) {
		RW_THROW("Weights and objectives size mismatch!");
	}
	
	return _do_combine(objectives);
}
