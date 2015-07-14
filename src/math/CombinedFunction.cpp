/**
 * @file CombinedFunction.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombinedFunction.hpp"

using namespace gripperz::math;
using namespace std;


CombinedFunction::CombinedFunction(MultiObjectiveFunction::Ptr multiObjFunc, CombineObjectives::Ptr combineMethod) :
	_multiObjFunc(multiObjFunc),
	_method(combineMethod)
{
}


CombinedFunction::~CombinedFunction() {
}


double CombinedFunction::evaluate(const std::vector<double>& x) {
	vector<double> objectives = _multiObjFunc->evaluate(x);
	
	double result = _method->combine(objectives);
	
	return result;
}
