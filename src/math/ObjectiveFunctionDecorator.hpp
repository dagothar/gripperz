/**
 * @file ObjectiveFunctionDecorator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "ObjectiveFunction.hpp"

namespace gripperz {
namespace math {
	
/**
 * @class ObjectiveFunctionDecorator
 * @brief Base class for objective function decorators.
 */
class ObjectiveFunctionDecorator : public ObjectiveFunction {
public:
	typedef rw::common::Ptr<ObjectiveFunctionDecorator> Ptr;
	
public:
	ObjectiveFunctionDecorator(ObjectiveFunction::Ptr f);
	virtual ~ObjectiveFunctionDecorator();
	
	//! Returns evaluation of provided vector x.
	virtual Scalar evaluate(const Vector& x);

	ObjectiveFunction::Ptr getObjectiveFunction() const { return _f; }
	void setObjectiveFunction(ObjectiveFunction::Ptr f) { _f = f; }

private:
	ObjectiveFunction::Ptr _f;
};

} /* math */
} /* gripperz */
