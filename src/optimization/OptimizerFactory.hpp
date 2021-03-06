/**
 * @file OptimizerFactory.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Optimizer.hpp"
#include "BOBYQAOptimizer.hpp"
#include "SimplexOptimizer.hpp"
#include "CoordinateDescentOptimizer.hpp"
#include "SimulatedAnnealingOptimizer.hpp"
#include "PowellOptimizer.hpp"
#include "RbfOptimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class OptimizerFactory
 * @brief Creates optimizers based on string.
 */
class OptimizerFactory {
public:
	/**
	 * @brief Returns optimizer based on name.
	 */
	static Optimizer::Ptr makeOptimizer(const std::string& id, unsigned dim);
	
	//! Creates BOBYQA optimizer.
	static BOBYQAOptimizer::Ptr makeBOBYQAOptimizer(unsigned dim, double rho_init=1e-1, double rho_stop=1e-3, unsigned max_fev=1e3);
	
	//! Creates Simplex optimizer.
	static SimplexOptimizer::Ptr makeSimplexOptimizer(double rho_init=1e-1, double rho_stop=1e-3, unsigned max_fev=1e3);
	
	//! Creates CD optimizer.
	static CoordinateDescentOptimizer::Ptr makeCoordinateDescentOptimizer(double rho_init=1e-1, double rho_stop=1e-3, unsigned max_fev=1e3);
	
	//! Creates SA optimizer.
	static SimulatedAnnealingOptimizer::Ptr makeSimulatedAnnealingOptimizer(double rho_init=1e-1, double rho_stop=1e-3, unsigned max_fev=1e3);
	
	//! Creates Powell optimizer.
	static PowellOptimizer::Ptr makePowellOptimizer(double rho_init=1e-1, double rho_stop=1e-3, unsigned max_fev=1e3);
        
        //! Creates RBF optimizer.
	static RbfOptimizer::Ptr makeRbfOptimizer(unsigned max_iter = 1e3, unsigned max_fev=1e3);
};

} /* optimization */
} /* gripperz */
