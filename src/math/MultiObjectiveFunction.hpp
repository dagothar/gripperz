/**
 * @file MultiObjectiveFunction.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <vector>

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
	virtual std::vector<double> evaluate(const std::vector<double>& x) = 0;
};

} /* math */
} /* gripperz */