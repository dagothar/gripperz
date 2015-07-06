/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <models/Gripper.hpp>
#include <context/TaskDescription.hpp>
#include <optimization/MultiObjectiveFunction.hpp>

namespace gripperz {
namespace evaluation {

/**
 * @class GripperObjectiveFunction
 * @brief
 */
class GripperObjectiveFunction : public optimization::MultiObjectiveFunction {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperObjectiveFunction> Ptr;
	
	//! Number of parameters.
	static const unsigned NParameters = 9;
	
	//! Number of objectives.
	static const unsigned NObjectives = 7;
	
public:
	GripperObjectiveFunction(context::TaskDescription::Ptr td);
	
	virtual ~GripperObjectiveFunction();
	
	/**
	 * @brief Evaluates gripper given as the parameter vector.
	 * 
	 * Takes following parameters (9):
	 * length, width, depth, chf. depth, chf. angle, cut depth, cut angle, cut tilt, tcp position
	 * 
	 * Returns following objectives (7):
	 * success, robustness, alignment, coverage, wrench, stress, volume
	 */
	virtual std::vector<double> operator()(const std::vector<double>& x);
	
	/**
	 * @brief Builds gripper object from parameters.
	 * @param parameters [in] parameters
	 * @return Gripper object
	 */
	static models::Gripper::Ptr parametersToGripper(const std::vector<double>& parameters);

private:
	context::TaskDescription::Ptr _td;
};

} /* evaluation */
} /* gripperz */