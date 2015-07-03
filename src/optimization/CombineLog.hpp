/**
 * @file CombineLog.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "CombineObjectives.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class CombineLog
 * @brief Combines objectives using logarithmic weighted sum.
 */
class CombineLog : public CombineObjectives {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CombineLog> Ptr;
	
public:
	CombineLog(const std::vector<double> weights = std::vector<double>());
	virtual ~CombineLog();
	
protected:
	virtual double _do_combine(const std::vector<double>& objectives);
};

} /* optimization */
} /* gripperz */
