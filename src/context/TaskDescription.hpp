/**
 * @file TaskDescription.hpp
 * @author Adam Wolniakowski
 */

#pragma once

#include "Context.hpp"

#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <util/XMLHelpers.hpp>
#include <evaluation/AlignmentCalculator.hpp>

namespace gripperz {
namespace context {

/**
 * @class TaskDescription
 * 
 * @brief Contains description of a task.
 */
class TaskDescription : public Context {
public:
	//! Smart pointer to this type of class.
	typedef rw::common::Ptr<TaskDescription> Ptr;

	/// Used for storing baseline and weights for quality measurements.
	typedef struct {
		double coverage;
		double success;
		double wrench;
		double topwrench;
		double robustness;
		double stress;
		double volume;
		double alignment;
	} Qualities;

	/// Used to store RANSAC stable pose models parameters for calculating alignment
	struct AlignmentModelParameters {
		int iterations;
		int minInliers;
		double dataThreshold;
		double modelThreshold;
		double maxModels;

		AlignmentModelParameters(
			int iter,
			int inliers,
			double data,
			double model,
			double nmodels
		) :
			iterations(iter),
			minInliers(inliers),
			dataThreshold(data),
			modelThreshold(model),
			maxModels(nmodels)
		{}
	};

public:
	//! Constructor
	TaskDescription(rwsim::dynamics::DynamicWorkCell::Ptr dwc);

	virtual ~TaskDescription();

	double getInterferenceLimit() const {
		return _interferenceLimit;
	}
	
	void setInterferenceLimit(double limit) {
		_interferenceLimit = limit;
	}

	double getWrenchLimit() const {
		return _wrenchLimit;
	}
	
	void setWrenchLimit(double limit) {
		_wrenchLimit = limit;
	}

	double getStressLimit() const {
		return _stressLimit;
	}
	
	void setStressLimit(double limit) {
		_stressLimit = limit;
	}
	
	double getVolumeLimit() const {
		return _volumeLimit;
	}
	
	void setVolumeLimit(double limit) {
		_volumeLimit = limit;
	}

	std::vector<rw::models::Object::Ptr> getInterferenceObjects() {
		return _interferenceObjects;
	}
	
	void addInterferenceObject(rw::models::Object::Ptr object) {
		_interferenceObjects.push_back(object);
	}

	bool hasHints() const {
		return _hints.size() > 0;
	}
	
	std::vector<rw::math::Transform3D<> >& getHints() {
		return _hints;
	}
	
	void addHint(rw::math::Transform3D<> hint) {
		_hints.push_back(hint);
	}
	
	rw::math::Q getPrefilteringDistance() const {
		return _prefilteringDistance;
	}
	
	void setPrefilteringDistance(rw::math::Q dist) {
		_prefilteringDistance = dist;
	}
	
	rw::math::Q getCoverageDistance() const {
		return _coverageDistance;
	}
	
	void setCoverageDistance(rw::math::Q dist) {
		_coverageDistance = dist;
	}
	
	rw::math::Q getTeachDistance() const {
		return _teachDistance;
	}
	
	void setTeachDistance(rw::math::Q dist) {
		_teachDistance = dist;
	}

	rw::models::Object::Ptr getTargetObject() {
		return _targetObject;
	}
	
	void setTargetObject(rw::models::Object::Ptr object) {
		_targetObject = object;
		_targetFrame = _targetObject->getBase();
	}
	
	void setTargetObject(const std::string& objectName) {
		setTargetObject(getWorkCell()->findObject(objectName));
	}
	
	rw::kinematics::Frame* getTargetFrame() {
		return _targetFrame;
	}
	
	void setTargetFrame(rw::kinematics::Frame* frame) {
		_targetFrame = frame;
	}

	const std::string& getGripperID() {
		return _gripperID;
	}
	
	void setGripperID(const std::string id) {
		_gripperID = id;
	}

	rw::models::TreeDevice::Ptr getGripperDevice() {
		return _gripperDevice;
	}
	
	void setGripperDevice(rw::models::TreeDevice::Ptr device) {
		_gripperDevice = device;
	}
	
	rwsim::dynamics::RigidDevice::Ptr getGripperDynamicDevice() {
		return _gripperDynamicDevice;
	}
	
	void setGripperDynamicDevice(rwsim::dynamics::RigidDevice::Ptr device) {
		_gripperDynamicDevice = device;
	}
	
	rw::kinematics::MovableFrame::Ptr getGripperTCP() {
		return _gripperTCP;
	}
	
	void setGripperTCP(rw::kinematics::MovableFrame::Ptr frame) {
		_gripperTCP = frame;
	}
	
	rw::kinematics::MovableFrame* getGripperMovable() {
		return _gripperMovable;
	}
	
	void setGripperMovable(rw::kinematics::MovableFrame* frame) {
		_gripperMovable = frame;
	}
	
	const std::string& getControllerID() {
		return _controllerID;
	}
	
	void setControllerID(const std::string& id) {
		_controllerID = id;
	}

	/**
	 * @brief Returns RANSAC parameters for finding stable pose model
	 *
	 * @return structure with (iterations, minInliers, dataThreshold, modelThreshold, maxModels)
	 */
	AlignmentModelParameters& getAlignmentParameters() {
		return _alignmentParameters;
	}
	
	double getAlignmentFilteringDistance() const { return _alignmentFilteringDistance; }
	void setAlignmentFilteringDistance(double dist) { _alignmentFilteringDistance = dist; }
	
	const std::string& getAlignmentCalculatorID() const { return _alignmentCalculatorID; }
	void setAlignmentCalculatorID(const std::string& id) { _alignmentCalculatorID = id; }
	
	//! Creates alignment calculator based on the configuration.
	evaluation::AlignmentCalculator::Ptr getAlignmentCalculator();

protected:
	// data
	double _interferenceLimit;
	double _wrenchLimit;
	double _stressLimit;
	double _volumeLimit;
	std::vector<rw::models::Object::Ptr> _interferenceObjects;
	rw::math::Q _prefilteringDistance; // distance used to filter samples at the start, so they are more uniform
	rw::math::Q _coverageDistance; // distance used for filtering grasps for coverage
	rw::math::Q _teachDistance;
	rw::models::Object::Ptr _targetObject;
	rw::kinematics::Frame* _targetFrame;
	std::string _gripperID;
	rw::models::TreeDevice::Ptr _gripperDevice;
	rw::kinematics::MovableFrame::Ptr _gripperTCP;
	rw::kinematics::MovableFrame* _gripperMovable;
	std::string _controllerID;
	rwsim::dynamics::RigidDevice::Ptr _gripperDynamicDevice;
	std::vector<rw::math::Transform3D<> > _hints;

	// ransac parameters for finding stable pose
	std::string _alignmentCalculatorID;
	AlignmentModelParameters _alignmentParameters;
	
	double _alignmentFilteringDistance;
};

} /* context */
} /* gripperz */
