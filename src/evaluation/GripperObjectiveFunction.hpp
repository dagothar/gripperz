/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <models/Gripper.hpp>
#include <models/GripperBuilder.hpp>
#include <evaluation/GripperEvaluationManager.hpp>
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
	GripperObjectiveFunction(models::GripperBuilder::Ptr builder, evaluation::GripperEvaluationManager::Ptr manager);
	
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
	
	models::GripperBuilder::Ptr getBuilder() { return _builder; }
	void setBuilder(models::GripperBuilder::Ptr builder) { _builder = builder; }
	
	evaluation::GripperEvaluationManager::Ptr getEvaluationManager() { return _manager; }
	void setEvaluationManager(evaluation::GripperEvaluationManager::Ptr manager) { _manager = manager; }

private:
	models::GripperBuilder::Ptr _builder;
	evaluation::GripperEvaluationManager::Ptr _manager;
};

} /* evaluation */
} /* gripperz */
