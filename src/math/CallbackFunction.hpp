/**
 * @file CallbackFunction.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#pragma once

#include "ObjectiveFunctionDecorator.hpp"
#include <boost/function.hpp>

namespace gripperz {
namespace math {

/**
 * @class CallbackFunction
 * @brief A decorator that provides callback with function evaluations.
 * A callback is called with two arguments: the vector of objective function arguments, and the objective function result.
 */
class CallbackFunction: public ObjectiveFunctionDecorator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CallbackFunction> Ptr;

public:
	CallbackFunction(ObjectiveFunction::Ptr f, boost::function<void(const Vector&, Scalar)> cb);
	
	virtual ~CallbackFunction();
	
	boost::function<void(const Vector&, Scalar)> getCallback() const { return _cb; }
	void setCallback(boost::function<void(const Vector&, Scalar)> cb) { _cb = cb; }
	
	//! Returns evaluation of provided vector x.
	virtual Scalar evaluate(const Vector& x);

private:
	boost::function<void(const Vector&, Scalar)> _cb;
};

} /* math */
} /* gripperz */
