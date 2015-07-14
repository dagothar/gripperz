/**
 * @file CallbackFunction.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#include "CallbackFunction.hpp"


using namespace gripperz::math;


CallbackFunction::CallbackFunction(ObjectiveFunction::Ptr f, boost::function<void(const Vector&, Scalar)> cb) :
	ObjectiveFunctionDecorator(f),
	_cb(cb)
{
}


CallbackFunction::~CallbackFunction() {
}


Scalar CallbackFunction::evaluate(const Vector& x) {
	Scalar result = ObjectiveFunctionDecorator::evaluate(x);
	
	if (_cb) {
		_cb(x, result);
	}
	
	return result;
}
