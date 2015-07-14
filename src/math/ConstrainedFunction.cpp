/**
 * @file ConstrainedFunction.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#include "ConstrainedFunction.hpp"


using namespace gripperz::math;


ConstrainedFunction::ConstrainedFunction(ObjectiveFunction::Ptr f) :
	ObjectiveFunctionDecorator(f)
{
}


ConstrainedFunction::~ConstrainedFunction() {
}
