/**
 * @file BOBYQAOptimizer.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#pragma once

#include "Optimizer.hpp"

namespace gripperz {
namespace optimization {

/**
 * @class BOBYQAOptimizer
 * @brief
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
	
	//! Constraint.
	typedef std::pair<double, double> Constraint;
	
	//! Constraints.
	typedef std::vector<Constraint> ConstraintList;

public:
	BOBYQAOptimizer(Configuration config, const ConstraintList& constr);
	
	virtual ~BOBYQAOptimizer();
	
	Configuration getConfiguration() const { return _config; }
	void setConfiguration(Configuration config) { _config = config; }
	
	ConstraintList& getConstraints() { return _constraints; }
	void setConstraints(const ConstraintList& constr) { _constraints = constr; }
	
	/**
	 * @brief Minimizes the objective function starting from the initial guess using BOBYQA method from dlib.
	 */
	virtual std::vector<double> minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);

private:
	Configuration _config;
	ConstraintList _constraints;
};

} /* optimization */
} /* gripperz */
