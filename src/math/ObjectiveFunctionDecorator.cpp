/**
 * @file ObjectiveFunctionDecorator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "ObjectiveFunctionDecorator.hpp"

using namespace gripperz::math;


ObjectiveFunctionDecorator::ObjectiveFunctionDecorator(ObjectiveFunction::Ptr f) :
	_f(f)
{
}


ObjectiveFunctionDecorator::~ObjectiveFunctionDecorator() {
}


double ObjectiveFunctionDecorator::evaluate(const std::vector<double>& x) {
	return _f->evaluate(x);
}
