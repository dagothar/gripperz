/**
 * @file CombineLog.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "CombineObjectives.hpp"

namespace gripperz {
namespace math {

/**
 * @class CombineLog
 * @brief Combines objectives using logarithmic weighted sum.
 */
class CombineLog : public CombineObjectives {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CombineLog> Ptr;
	
public:
	CombineLog(const Vector& weights = Vector());
	virtual ~CombineLog();
	
protected:
	virtual Scalar _do_combine(const Vector& objectives);
};

} /* math */
} /* gripperz */
