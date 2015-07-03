/**
 * @file MappedFunction.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "MappedFunction.hpp"

using namespace gripperz::optimization;


MappedFunction::MappedFunction(ObjectiveFunction::Ptr f, ParameterMapping::Ptr mapping) :
	ObjectiveFunctionDecorator(f),
	_mapping(mapping)
{}


MappedFunction::~MappedFunction() {
}


double MappedFunction::operator()(const std::vector<double>& x) {
	std::vector<double> unpacked = _mapping->unpack(x);
	
	return ObjectiveFunctionDecorator::operator()(unpacked);
}
