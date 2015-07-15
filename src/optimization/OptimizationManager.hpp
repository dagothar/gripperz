/**
 * @file OptimizationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#pragma once

#include "Optimizer.hpp"
#include <string>
#include <math/ParameterMapping.hpp>

namespace gripperz {
namespace optimization {

/**
 * @class OptimizationManager
 * @brief Manages optimization of the objective function, providing utilities.
 * Utilities include: mapping function parameters into an uniform space, and
 * logging optimization progress.
 */
class OptimizationManager {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<OptimizationManager> Ptr;
	
	//! Log entry.
	typedef std::pair<math::Vector, math::Scalar> LogEntry;
	
	//! Log.
	typedef std::vector<LogEntry> Log;

public:
	/**
	 * @brief Constructor.
	 * @param optimizer [in] optimizer to use
	 * @param ranges [in] list of parameter ranges, used for mapping function internally (optional)
	 */
	OptimizationManager(Optimizer::Ptr optimizer, const math::RangeList& ranges=math::RangeList());
	
	virtual ~OptimizationManager();
	
	Optimizer::Ptr getOptimizer() { return _optimizer; }
	void setOptimizer(Optimizer::Ptr opt) { _optimizer = opt; }
	
	math::RangeList& getRanges() { return _ranges; }
	void setRanges(const math::RangeList& ranges) { _ranges = ranges; }
	
	Log& getLog() { return _log; }
	
	/**
	 * @brief Optimizes given objective function starting from the initialGuess.
	 * @param function [in] a function to optimize
	 * @param initialGuess [in] starting point
	 * @param dir [in] optimization direction (minimize / maximize)
	 */
	virtual math::Vector optimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess, const std::string& dir="minimize");

protected:
	/**
	 * @brief Callback for evaluated functions.
	 * By default, stores arguments and results in the log.
	 */
	virtual void callback(const math::Vector& args, math::Scalar result);

private:
	Optimizer::Ptr _optimizer;
	math::RangeList _ranges;
	Log _log;
};

} /* optimization */
} /* gripperz */
