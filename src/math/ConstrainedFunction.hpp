/**
 * @file ConstrainedFunction.hpp
 * @author Adam Wolniakowski
 * @date 14-07-2015
 */

#pragma once

#include "ObjectiveFunctionDecorator.hpp"
#include "Constraint.hpp"

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
	ConstrainedFunction(ObjectiveFunction::Ptr f, Constraint::Ptr constraint, Scalar defaultValue=0.0);
	virtual ~ConstrainedFunction();
	
	Constraint::Ptr getConstraint() { return _constraint; }
	void setConstraint(Constraint::Ptr constr) { _constraint = constr; }
	
	Scalar getDefaultValue() const { return _defaultValue; }
	void setDefaultValue(Scalar val) { _defaultValue = val; }
	
	/**
	 * @brief Returns evaluation of provided vector x.
	 * If x doesn't fit the constraint, a default value is returned
	 */
	virtual Scalar evaluate(const Vector& x);

private:
	Constraint::Ptr _constraint;
	Scalar _defaultValue;
};

} /* math */
} /* gripperz */
