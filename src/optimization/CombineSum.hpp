/**
 * @file CombineSum.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "CombineObjectives.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class CombineSum
 * @brief Combines objectives using arithmetic weighted sum.
 */
class CombineSum : public CombineObjectives {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CombineSum> Ptr;
	
public:
	CombineSum(const std::vector<double> weights = std::vector<double>());
	virtual ~CombineSum();

protected:
	virtual double _do_combine(const std::vector<double>& objectives);
};

} /* optimization */
} /* gripperz */
