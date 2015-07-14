/**
 * @file RevertedFunction.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "RevertedFunction.hpp"

using namespace gripperz::math;


RevertedFunction::RevertedFunction(ObjectiveFunction::Ptr f) :
	ObjectiveFunctionDecorator(f)
{}


RevertedFunction::~RevertedFunction() {
}


Scalar RevertedFunction::evaluate(const Vector& x) {
	return -ObjectiveFunctionDecorator::evaluate(x);
}
