/**
 * @file ObjectiveFunction.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <vector>

namespace gripperz {
namespace optimization {
	
/**
 * @class ObjectiveFunction
 * @brief Interface for single objective functions.
 */
class ObjectiveFunction {
public:
	typedef rw::common::Ptr<ObjectiveFunction> Ptr;
	
public:
	//! Returns evaluation of provided vector x.
	virtual double evaluate(const std::vector<double>& x) = 0;
};

} /* optimization */
} /* gripperz */
