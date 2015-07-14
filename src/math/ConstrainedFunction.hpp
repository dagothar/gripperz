/**
 * @file ConstrainedFunction.hpp
 * @author Adam Wolniakowski
 * @date 14-07-2015
 */

#pragma once

#include "ObjectiveFunctionDecorator.hpp"

namespace gripperz {
namespace math {
	
/**
 * @class ConstrainedFunction
 * @brief Decorator that wraps ObjectiveFunction and returns a constant result when outside the bounds.
 */
class ConstrainedFunction : public ObjectiveFunctionDecorator {
public:
	typedef rw::common::Ptr<ConstrainedFunction> Ptr;
	
public:
	ConstrainedFunction(ObjectiveFunction::Ptr f);
	virtual ~ConstrainedFunction();
	
	//! Returns evaluation of provided vector x.
	virtual double evaluate(const std::vector<double>& x);

private:
};

} /* math */
} /* gripperz */
