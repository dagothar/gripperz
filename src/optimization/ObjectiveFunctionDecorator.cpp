/**
 * @file ObjectiveFunctionDecorator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "ObjectiveFunctionDecorator.hpp"

using namespace gripperz::optimization;


ObjectiveFunctionDecorator::ObjectiveFunctionDecorator(ObjectiveFunction::Ptr f) :
	_f(f)
{
}


ObjectiveFunctionDecorator::~ObjectiveFunctionDecorator() {
}


double ObjectiveFunctionDecorator::operator()(const std::vector<double>& x) {
	return _f->operator()(x);
}
