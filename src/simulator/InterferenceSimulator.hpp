/**
 * @file InterferenceSimulator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "BasicSimulator.hpp"

#include <rwsim/simulator/GraspTaskSimulator.hpp>

namespace gripperz {
namespace simulator {

/**
 * @class InterferenceSimulator
 * @brief Extends the BasicSimulator with checking the interference limit.
 */
class InterferenceSimulator : public BasicSimulator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<InterferenceSimulator> Ptr;

public:
	InterferenceSimulator(rwsim::dynamics::DynamicWorkCell::Ptr dwc, unsigned threads=1);
	virtual ~InterferenceSimulator();
	
	double getInterferenceLimit() const { return _interferenceLimit; }
	void setInterferenceLimit(double limit) { _interferenceLimit = limit; }
	
	std::vector<rw::models::Object::Ptr>& getInterferenceObjects() { return _interferenceObjects; }
	void setInterferenceObjects(const std::vector<rw::models::Object::Ptr>& objects) { _interferenceObjects = objects; }

protected:
	//! Inherited from GraspTaskSimulator
	virtual void graspFinished(SimState& sstate);
	
private:
	double _interferenceLimit;
	std::vector<rw::models::Object::Ptr> _interferenceObjects;
};

} /* simulator */
} /* gripperz */
