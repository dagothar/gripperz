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


Scalar CombinedFunction::evaluate(const Vector& x) {
	Vector objectives = _multiObjFunc->evaluate(x);
	
	Scalar result = _method->combine(objectives);
	
	return result;
}
