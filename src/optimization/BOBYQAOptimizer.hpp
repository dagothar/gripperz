/**
 * @file BOBYQAOptimizer.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#pragma once

#include "Optimizer.hpp"
#include <math/ParameterMapping.hpp>

namespace gripperz {
namespace optimization {

/**
 * @class BOBYQAOptimizer
 * @brief Uses dlib's BOBYQA optimizer.
 * Maps the objective function internally, so that the optimizer always operates
 * in the box (0, 1)^n.
 * Uses constraints to create the mapping.
 */
class BOBYQAOptimizer: public Optimizer {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<BOBYQAOptimizer> Ptr;
	
	//! Configuration.
	struct Configuration {
		double initialTrustRegionRadius;
		double finalTrustRegionRadius;
		unsigned maxNOfEvaluations;
		
		Configuration() :
			initialTrustRegionRadius(0.1),
			finalTrustRegionRadius(1e-3),
			maxNOfEvaluations(1000)
		{}
	};
	
	//! Constraints.
	typedef std::vector<math::Range> ConstraintList;

public:
	BOBYQAOptimizer(Configuration config, const ConstraintList& constr);
	
	virtual ~BOBYQAOptimizer();
	
	Configuration getConfiguration() const { return _config; }
	void setConfiguration(Configuration config) { _config = config; }
	
	ConstraintList& getConstraints() { return _constraints; }
	void setConstraints(const ConstraintList& constr);
	
	/**
	 * @brief Minimizes the objective function starting from the initial guess using BOBYQA method from dlib.
	 */
	virtual math::Vector minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);

protected:
	//! Creates parameter mapping based on constraints.
	virtual math::ParameterMapping::Ptr makeMapping(const ConstraintList& constr);

private:
	Configuration _config;
	ConstraintList _constraints;
	math::ParameterMapping::Ptr _mapping;
};

} /* optimization */
} /* gripperz */
