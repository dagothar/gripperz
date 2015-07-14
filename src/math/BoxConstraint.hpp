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
	BoxConstraint(const RangeList& ranges);
	
	virtual ~BoxConstraint();
	
	RangeList getRanges() const { return _ranges; }
	void setRanges(const RangeList& ranges) { _ranges = ranges; }
	
	/**
	 * @brief Tests whether a vector of parameters matches this constraint.
	 */
	virtual bool isMetBy(const Vector& params);

private:
	RangeList _ranges;
};

} /* math */
} /* gripperz */
