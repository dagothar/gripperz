/**
 * @file RevertedFunction.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "ObjectiveFunctionDecorator.hpp"

namespace gripperz {
namespace math {
	
/**
 * @class RevertedFunction
 * @brief Decorator applies a -1 multiplier to an ObjectiveFunction.
 * Useful for finding maximum with a minimizer.
 */
class RevertedFunction : public ObjectiveFunctionDecorator {
public:
	typedef rw::common::Ptr<RevertedFunction> Ptr;
	
public:
	RevertedFunction(ObjectiveFunction::Ptr f);
	virtual ~RevertedFunction();
	
	//! Returns -evaluation of provided vector x.
	virtual double evaluate(const std::vector<double>& x);
};

} /* math */
} /* gripperz */
