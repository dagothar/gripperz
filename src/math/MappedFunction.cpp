/**
 * @file MappedFunction.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "MappedFunction.hpp"

using namespace gripperz::math;


MappedFunction::MappedFunction(ObjectiveFunction::Ptr f, ParameterMapping::Ptr mapping) :
	ObjectiveFunctionDecorator(f),
	_mapping(mapping)
{}


MappedFunction::~MappedFunction() {
}


Scalar MappedFunction::evaluate(const Vector& x) {
	Vector unpacked = _mapping->map(x);
	
	return ObjectiveFunctionDecorator::evaluate(unpacked);
}
