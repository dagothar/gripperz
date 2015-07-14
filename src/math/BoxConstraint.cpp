/**
 * @file BoxConstraint.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#include "BoxConstraint.hpp"
#include <rw/common/macros.hpp>


using namespace gripperz::math;


BoxConstraint::BoxConstraint(const RangeList& ranges) : _ranges(ranges) {
}


BoxConstraint::~BoxConstraint() {
}


bool BoxConstraint::isMetBy(const Vector& params) {
	if (params.size() != _ranges.size()) {
		RW_THROW ("Parameters and ranges size mismatch!");
	}
	
	for (unsigned i = 0; i < params.size(); ++i) {
		if (params[i] < _ranges[i].first || params[i] > _ranges[i].second) {
			return false;
		}
	}
	
	return true;
}
