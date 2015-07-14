/**
 * @file ParameterMapping.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "ParameterMapping.hpp"

#include <rw/common/macros.hpp>


using namespace gripperz::math;


ParameterMapping::ParameterMapping(const Map& map) :
	_map(map)
{}


ParameterMapping::~ParameterMapping() {
}


double mapParameter(double p, const Range& before, const Range& after) {
	/* what is the position of p value now? */
	double p0 = (p - before.first) / (before.second - before.first);
	
	/* what should it be after? */
	double p1 = after.first + p0 * (after.second - after.first);
	
	return p1;
}


Vector ParameterMapping::map(const Vector& params) const {
	if (params.size() != _map.size()) {
		RW_THROW("Map and parameters size mismatch!");
	}
	
	Vector m(params.size());
	for (unsigned i = 0; i < m.size(); ++i) {
		m[i] = mapParameter(params[i], _map[i].first, _map[i].second);
	}
	
	return m;
}

	
Vector ParameterMapping::unmap(const Vector& params) const {
	if (params.size() != _map.size()) {
		RW_THROW("Map and parameters size mismatch!");
	}
	
	Vector m(params.size());
	for (unsigned i = 0; i < m.size(); ++i) {
		m[i] = mapParameter(params[i], _map[i].second, _map[i].first);
	}
	
	return m;
}
