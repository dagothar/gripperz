/**
 * @file SimplexOptimizer.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#pragma once

#include "Optimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class SimplexOptimizer
 * @brief Optimizes using Nelder-Mead method.
 */
class SimplexOptimizer: public Optimizer {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<SimplexOptimizer> Ptr;

public:
	SimplexOptimizer();
	
	virtual ~SimplexOptimizer();
};

} /* optimization */
} /* gripperz */
