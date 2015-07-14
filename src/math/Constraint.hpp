/**
 * @file Constraint.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Types.hpp"

namespace gripperz {
namespace math {

/**
 * @class Constraint
 * @brief Interface for mathematical concept of constraint.
 * Tests whether a vector of parameters matches the constraint.
 */
class Constraint {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<Constraint> Ptr;

public:
	/**
	 * @brief Tests whether a vector of parameters matches this constraint.
	 */
	virtual bool isMetBy(const Vector& params) = 0;
};

} /* math */
} /* gripperz */
