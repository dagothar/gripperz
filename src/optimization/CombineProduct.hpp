/**
 * @file CombineProduct.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "CombineObjectives.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class CombineProduct
 * @brief Combines objectives using geometric weighted sum.
 */
class CombineProduct : public CombineObjectives {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CombineProduct> Ptr;
	
public:
	CombineProduct(const std::vector<double> weights = std::vector<double>());
	virtual ~CombineProduct();
	
protected:
	virtual double _do_combine(const std::vector<double>& objectives);
};

} /* optimization */
} /* gripperz */
