/**
 * @file CombinedFunction.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombinedFunction.hpp"

using namespace gripperz::optimization;


CombinedFunction::CombinedFunction(MultiObjectiveFunction::Ptr multiObjFunc, CombineObjectives::Ptr combineMethod) :
	_multiObjFunc(multiObjFunc),
	_method(combineMethod)
{
}


CombinedFunction::~CombinedFunction() {
}


double CombinedFunction::operator()(const std::vector<double>& x) {
	return _method->combine(_multiObjFunc->operator()(x));
}
