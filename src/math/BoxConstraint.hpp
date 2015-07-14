/**
 * @file BoxConstraint.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#pragma once

#include "Constraint.hpp"

namespace gripperz {
namespace math {

/**
 * @class BoxConstraint
 * @brief Implements a box type of constraint defined by upper and lower bounds.
 */
class BoxConstraint: public Constraint {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<BoxConstraint> Ptr;

public:
	BoxConstraint();
	
	virtual ~BoxConstraint();
	
	/**
	 * @brief Tests whether a vector of parameters matches this constraint.
	 */
	virtual bool isMetBy(const Vector& params);
};

} /* math */
} /* gripperz */
