/**
 * @file Gripper.hpp
 * @author Adam Wolniakowski
 */

#pragma once

#include <iostream>
#include <rw/common/Ptr.hpp>
#include <rw/math/Q.hpp>
#include <rw/math/Transform3D.hpp>
#include <rw/models/WorkCell.hpp>
#include <rw/models/TreeDevice.hpp>
#include <rw/graphics/WorkCellScene.hpp>
#include <rw/proximity/CollisionDetector.hpp>
#include <rwsim/rwsim.hpp>
#include <rwlibs/task.hpp>
#include <rwsim/rwsim.hpp>
#include <rwsim/simulator/GraspTaskSimulator.hpp>

#include <geometry/JawPrimitive.hpp>
#include <context/TaskDescription.hpp>

namespace rw {
namespace kinematics {
class Frame;
class State;
}
}

namespace gripperz {

namespace loaders {
class GripperXMLLoader;
} /* loaders */

namespace models {

/**
 * Contains detailed evaluation of specific gripper design.
 * 
 * @todo ...
 */
struct GripperQuality {
	typedef rw::common::Ptr<GripperQuality> Ptr;

	GripperQuality() :
			nOfExperiments(0), nOfSuccesses(0), nOfSamples(0),
			//shape(0.0),
			coverage(0.0), success(0.0), wrench(0.0), topwrench(0.0), robustness(
					0.0), maxstress(0.0), volume(0.0), alignment(0.0), quality(0.0) {
	}

	friend std::ostream& operator<<(std::ostream& stream,
			const GripperQuality& q) {
		stream << "GripperQuality:\n" << "- experiments= " << q.nOfExperiments
				<< '\n' << "- successes= " << q.nOfSuccesses << '\n'
				<< "- samples= " << q.nOfSamples << '\n'
				<< "- coverage= " << q.coverage << '\n' << "- success= "
				<< q.success << '\n' << "- wrench= " << q.wrench << '\n'
				<< "- topwrench= " << q.topwrench << '\n' << "- robustness= "
				<< q.robustness << '\n' << "- maxstress= " << q.maxstress
				<< '\n' << "- alignment= " << q.alignment << '\n'
				<< "- volume= " << q.volume << '\n' << "- quality= "
				<< q.quality << std::endl;

		return stream;
	}

	// data	
	int nOfExperiments; // Number of performed experiments.

	int nOfSuccesses; // Number of succesful grasps (filtered).
	int nOfSamples; // Number of generated samples (filtered).

	double coverage; // Ratio of filtered succesful grasps to filtered all samples.
	double success; // Ratio of succesful grasps to all generated grasps.
	double wrench; // Average wrench of succesful grasps.
	double topwrench; // Average quality of top 20% of grasps.
	double robustness; // Robustness of succesful grasps.
	double maxstress; // Max. stress a gripper takes.
	double volume; // The volume of the gripper's jaw.
	double alignment; // Aligning property of the gripper.
	double quality; // Ultimate measurement of gripper quality.
};

/**
 * @class Gripper
 * @brief Gripper device (parallel jaw gripper) with parametrized geometry and kinematic and dynamic parameters.
 * 
 * Gripper is described by its geometric and kinematic & dynamic parameters. It contains geometries for jaws (shared between
 * left & right jaw) and gripper base. These geometries can either be initialized from mesh (e.g. from STL file) or
 * procedurally generated using JawPrimitive and Box primitive for fingers and base respectively.
 * Kinematic & dynamic parameters include:
 * - min. and max. opening
 * - max. force
 * - TCP position
 * 
 * Gripper contains a GripperQuality structure which describes gripper's qualities.
 * 
 * Default gripper constructor creates gripper with following parameters:
 * ...
 */
class Gripper
{
public:
	/// Smart pointer.
	typedef rw::common::Ptr<Gripper> Ptr;

public:
	Gripper(const std::string& name="gripper");

	virtual ~Gripper() {
	}

	std::string getName() const { return _name;	}
	void setName(const std::string& name) {	_name = name; }
	
	double getLength() const { return _length; }
	void setLength(double x) { _length = x; }
	
	double getWidth() const { return _width; }
	void setWidth(double x) { _width = x; }
	
	double getDepth() const { return _depth; }
	void setDepth(double x) { _depth = x; }
	
	double getChfDepth() const { return _chfdepth; }
	void setChfDepth(double x) { _chfdepth = x; }
	
	double getChfAngle() const { return _chfangle; }
	void setChfAngle(double x) { _chfangle = x; }
	
	double getCutDepth() const { return _cutdepth; }
	void setCutDepth(double x) { _cutdepth = x; }
	
	double getCutAngle() const { return _cutangle; }
	void setCutAngle(double x) { _cutangle = x; }
	
	double getCutTilt() const { return _cuttilt; }
	void setCutTilt(double x) { _cuttilt = x; }

	double getForce() const { return _force; }
	void setForce(double force) { _force = force; }

	double getTcpOffset() const { return _tcpoffset; }
	void setTcpOffset(double tcp) {	_tcpoffset = tcp; }

	double getJawdist() const {	return _jawdist; }
	void setJawdist(double jawdist) { _jawdist = jawdist; }

	double getStroke() const { return _stroke; }
	void setStroke(double stroke) {	_stroke = stroke; }
	
	double getBaseX() const { return _basex; }
	void setBaseX(double x) { _basex = x; }
	
	double getBaseY() const { return _basey; }
	void setBaseY(double y) { _basey = y; }
	
	double getBaseZ() const { return _basez; }
	void setBaseZ(double z) { _basez = z; }

	/**
	 * @brief Creates finger geometry
	 */
	rw::geometry::Geometry::Ptr getFingerGeometry();

	/**
	 * @brief Creates base geometry.
	 */
	rw::geometry::Geometry::Ptr getBaseGeometry();

	/**
	 * @brief Updates selected gripper device in the workcell according to data in this class.
	 *
	 * THIS IS A DIRTY HACK.
	 * Basically, workcell contains a skeleton of gripper device and dynamic gripper device. What this class and
	 * method does, is to dress this skeleton up. Geometries are removed and replaced in:
	 * - workcell
	 * - workcellscene (for displaying purposes)
	 * - collision detector
	 * - also dynamicworkcell has internal storage for some internal purposes apparently
	 *
	 * This was done as the easiest way to try different gripper designs in the same workcell without
	 * having it reloaded, which requires restarting RobWorkStudio. Another approach would be to
	 * add gripper to the workcell and then remove it, but methods neccessary for are not yet implemented.
	 *
	 * @todo Make this use only TaskDescription.
	 */
	void updateGripper(
		rw::models::WorkCell::Ptr wc,
		rwsim::dynamics::DynamicWorkCell::Ptr dwc,
		rw::models::TreeDevice::Ptr dev,
		rwsim::dynamics::RigidDevice::Ptr ddev,
		rw::kinematics::State& state,
		context::TaskDescription::Ptr td
	);

	GripperQuality& getQuality() { return _quality; }
	void setQuality(const GripperQuality& quality) { _quality = quality; }

	/**
	 * @brief Calculates parametrized gripper's crossection breadth at the point along its length.
	 *
	 * @param x [in] distance from the base
	 *
	 * @return crossection height
	 */
	double getCrossHeight(double x) const;

	/**
	 * @brief Calculates worst case maximum stress for the gripper.
	 *
	 * It's calculated by assuming max. grippers force acting on the tip (which gives the largest moment).
	 *
	 * @return Stress.
	 */
	double getMaxStress() const;

	/**
	 * @brief Calculates the volume of the gripper's jaw outline.
	 *
	 * This is an estimation of material neccesary to make the finger.
	 *
	 * @return Volume [m^3].
	 */
	double getVolume() const;

	// friends
	friend class loaders::GripperXMLLoader;

private:
	std::string _name;
	
	/* parameters */
	double _length;
	double _width;
	double _depth;
	double _chfdepth;
	double _chfangle;
	double _cutdepth;
	double _cutangle;
	double _cuttilt;
	double _tcpoffset;
	double _jawdist;
	double _stroke;
	double _force;
	double _basex;
	double _basey;
	double _basez;
	
	/* quality */
	GripperQuality _quality;
};

} /* models */
} /* gripperz */
