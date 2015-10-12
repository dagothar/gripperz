/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <models/OldGripper.hpp>
#include <models/GripperBuilder.hpp>
#include <evaluation/GripperEvaluationManager.hpp>
#include <evaluation/GripperQualityExtractor.hpp>
#include <math/MultiObjectiveFunction.hpp>
#include <boost/function.hpp>

namespace gripperz {
namespace evaluation {

/**
 * @class GripperObjectiveFunction
 * @brief
 */
class GripperObjectiveFunction : public math::MultiObjectiveFunction {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<GripperObjectiveFunction> Ptr;
	
	//! Number of parameters.
	static const unsigned NParameters = 9;
	
	//! Number of objectives.
	static const unsigned NObjectives = 7;
	
public:
	GripperObjectiveFunction(models::GripperBuilder::Ptr builder, GripperEvaluationManager::Ptr manager, GripperQualityExtractor::Ptr extractor=rw::common::ownedPtr(new GripperQualityExtractor));
	
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
	virtual std::vector<double> evaluate(const std::vector<double>& x);
	
	models::GripperBuilder::Ptr getBuilder() { return _builder; }
	void setBuilder(models::GripperBuilder::Ptr builder) { _builder = builder; }
	
	GripperEvaluationManager::Ptr getEvaluationManager() { return _manager; }
	void setEvaluationManager(GripperEvaluationManager::Ptr manager) { _manager = manager; }
	
	GripperQualityExtractor::Ptr getQualityExtractor() { return _extractor; }
	void setQualityExtractor(GripperQualityExtractor::Ptr extractor) { _extractor = extractor; }
	
	//! Returns last evaluated gripper.
	models::OldGripper::Ptr getLastGripper() const { return _lastGripper; }
	
	//! Returns last obtained gripper quality.
	models::GripperQuality::Ptr getLastGripperQuality() const { return _lastQuality; }
	
	//! Sets function evaluation callback.
	void setCallback(const boost::function<void(models::OldGripper::Ptr)>& cb) { _callback = cb; }

private:
	models::GripperBuilder::Ptr _builder;
	GripperEvaluationManager::Ptr _manager;
	GripperQualityExtractor::Ptr _extractor;
	
	models::OldGripper::Ptr _lastGripper;
	models::GripperQuality::Ptr _lastQuality;
	
	boost::function<void(models::OldGripper::Ptr)> _callback;
};

} /* evaluation */
} /* gripperz */
