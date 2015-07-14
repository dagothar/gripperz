/**
 * @file ObjectiveFunction.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Types.hpp"

namespace gripperz {
namespace math {
	
/**
 * @class ObjectiveFunction
 * @brief Interface for single objective functions.
 */
class ObjectiveFunction {
public:
	typedef rw::common::Ptr<ObjectiveFunction> Ptr;
	
public:
	//! Returns evaluation of provided vector x.
	virtual Scalar evaluate(const Vector& x) = 0;
};

} /* math */
} /* gripperz */
