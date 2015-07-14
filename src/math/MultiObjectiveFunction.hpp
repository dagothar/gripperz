/**
 * @file MultiObjectiveFunction.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Types.hpp"

namespace gripperz {
namespace math {
	
/**
 * @class MultiObjectiveFunction
 * @brief Interface for multi objective functions.
 */
class MultiObjectiveFunction {
public:
	typedef rw::common::Ptr<MultiObjectiveFunction> Ptr;
	
public:
	//! Returns evaluation of provided vector x.
	virtual Vector evaluate(const Vector& x) = 0;
};

} /* math */
} /* gripperz */
