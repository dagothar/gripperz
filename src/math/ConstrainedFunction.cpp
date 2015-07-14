/**
 * @file ConstrainedFunction.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#include "ConstrainedFunction.hpp"


using namespace gripperz::math;


ConstrainedFunction::ConstrainedFunction(ObjectiveFunction::Ptr f, Constraint::Ptr constraint, Scalar defaultValue) :
	ObjectiveFunctionDecorator(f),
	_constraint(constraint),
	_defaultValue(defaultValue)
{
}


ConstrainedFunction::~ConstrainedFunction() {
}


Scalar ConstrainedFunction::evaluate(const Vector& x) {
	if (!_constraint->isMetBy(x)) {
		return _defaultValue;
	}
	
	return ObjectiveFunctionDecorator::evaluate(x);
}
