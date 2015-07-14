/**
 * @file CombineProduct.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "CombineObjectives.hpp"

namespace gripperz {
namespace math {

/**
 * @class CombineProduct
 * @brief Combines objectives using geometric weighted sum.
 */
class CombineProduct : public CombineObjectives {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CombineProduct> Ptr;
	
public:
	CombineProduct(const Vector& weight =Vector());
	virtual ~CombineProduct();
	
protected:
	virtual Scalar _do_combine(const Vector& objectives);
};

} /* math */
} /* gripperz */
