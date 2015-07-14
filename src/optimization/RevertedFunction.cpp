/**
 * @file RevertedFunction.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "RevertedFunction.hpp"

using namespace gripperz::optimization;


RevertedFunction::RevertedFunction(ObjectiveFunction::Ptr f) :
	ObjectiveFunctionDecorator(f)
{}


RevertedFunction::~RevertedFunction() {
}


double RevertedFunction::evaluate(const std::vector<double>& x) {
	return -ObjectiveFunctionDecorator::evaluate(x);
}
