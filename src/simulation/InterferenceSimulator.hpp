/**
 * @file InterferenceSimulator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "BasicSimulator.hpp"

#include <rw/math/Metric.hpp>
#include <rw/math/MetricFactory.hpp>

namespace gripperz {
namespace simulation {

/**
 * @class InterferenceSimulator
 * @brief Extends the BasicSimulator with checking the interference limit.
 */
class InterferenceSimulator : public BasicSimulator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<InterferenceSimulator> Ptr;
	
	//! Metric used to calculate interference.
	typedef rw::math::Metric<rw::math::Transform3D<> > InterferenceMetric;

public:
	InterferenceSimulator(
		rwsim::dynamics::DynamicWorkCell::Ptr dwc,
		double limit,
		const std::vector<rw::models::Object::Ptr>& objects,
		InterferenceMetric::Ptr metric = rw::math::MetricFactory::makeTransform3DMetric<double>(1.0, 1.0),
		unsigned threads=1
	);
	
	virtual ~InterferenceSimulator();
	
	double getInterferenceLimit() const { return _interferenceLimit; }
	void setInterferenceLimit(double limit) { _interferenceLimit = limit; }
	
	std::vector<rw::models::Object::Ptr>& getInterferenceObjects() { return _interferenceObjects; }
	void setInterferenceObjects(const std::vector<rw::models::Object::Ptr>& objects) { _interferenceObjects = objects; }
	
	InterferenceMetric::Ptr getMetric() { return _metric; }
	void setMetric(InterferenceMetric::Ptr metric) { _metric = metric; }

protected:
	/**
	 * @brief Evaluates a grasp and changes the status if necessary.
	 * Changes status to Interference, if interference limit violated.
	 */
	virtual void evaluateGrasp(SimState& sstate);
	
	/**
	 * @brief Calculates interference between states in the whole scene.
	 */
	virtual double calculateSceneInterference(const rw::kinematics::State& state0, const rw::kinematics::State& state1) const;
	
	/**
	 * @brief Calculates interference between states for a single object.
	 */
	virtual double calculateObjectInterference(rw::models::Object::Ptr object, const rw::kinematics::State& state0, const rw::kinematics::State& state1) const;
	
private:
	double _interferenceLimit;
	std::vector<rw::models::Object::Ptr> _interferenceObjects;
	InterferenceMetric::Ptr _metric;
};

} /* simulation */
} /* gripperz */
