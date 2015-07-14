/**
 * @file MappedFunction.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "ObjectiveFunctionDecorator.hpp"
#include "ParameterMapping.hpp"

namespace gripperz {
namespace math {
	
/**
 * @class MappedFunction
 * @brief Decorator that wraps ObjectiveFunction and unpacks parameters before call using ParameterMapping.
 */
class MappedFunction : public ObjectiveFunctionDecorator {
public:
	typedef rw::common::Ptr<MappedFunction> Ptr;
	
public:
	MappedFunction(ObjectiveFunction::Ptr f, ParameterMapping::Ptr mapping);
	virtual ~MappedFunction();
	
	ParameterMapping::Ptr getMapping() const { return _mapping; }
	void setParameterMapping(ParameterMapping::Ptr mapping) { _mapping = mapping; }
	
	//! Returns evaluation of provided vector x after un-packing normalized parameters first.
	virtual Scalar evaluate(const Vector& x);

private:
	ParameterMapping::Ptr _mapping;
};

} /* math */
} /* gripperz */
