/**
 * @file CombineSum.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "CombineObjectives.hpp"

namespace gripperz {
namespace math {

/**
 * @class CombineSum
 * @brief Combines objectives using arithmetic weighted sum.
 */
class CombineSum : public CombineObjectives {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CombineSum> Ptr;
	
public:
	CombineSum(const Vector& weights=Vector());
	virtual ~CombineSum();

protected:
	virtual Scalar _do_combine(const Vector& objectives);
};

} /* math */
} /* gripperz */
