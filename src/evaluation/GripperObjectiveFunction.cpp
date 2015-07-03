/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "GripperObjectiveFunction.hpp"

#include <rwlibs/task/GraspTask.hpp>
#include <grasps/TaskGenerator.hpp>
#include <simulator/GripperTaskSimulator.hpp>


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulator;
using namespace gripperz::grasps;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;


GripperObjectiveFunction::GripperObjectiveFunction(context::TaskDescription::Ptr td) :
	_td(td)
{
}


GripperObjectiveFunction::~GripperObjectiveFunction() {
}


bool areParametersOk(const std::vector<double>& p) {
	// if tcp > length
	if (p[8] > p[0]) return false;
	
	// if cut depth > width
	if (p[5] > p[2]) return false;
	
	// check if cutout doesn't fall under chamfer
	double chflength = p[3] * p[1] * sin(p[4] * Deg2Rad);
	if (p[8] > p[0] - chflength) {
		double y = p[1] - (p[8] - chflength) * cos(p[4] * Deg2Rad);
		if (p[5] > y) return false;
	}
	
	return true;
}


std::vector<double> GripperObjectiveFunction::operator()(const std::vector<double>& x) {
	vector<double> results(NObjectives, 0.0);
	
	if (!areParametersOk(x)) {
		return results;
	}
	
	/*
	 * Build gripper.
	 */
	Gripper::Ptr gripper = NULL;
	try {
		gripper = parametersToGripper(x);
		gripper->updateGripper(
			_td->getWorkCell(),
			_td->getDynamicWorkCell(),
			_td->getGripperDevice(),
			_td->getGripperDynamicDevice(),
			_td->getInitState(),
			_td
		);
		_td->getInitState().upgrade();
	} catch (...) {
		RW_THROW("Exception during gripper generation!");
	}
	
	/*
	 * Generate grasps.
	 */
	GraspTask::Ptr targets = NULL;
	GraspTask::Ptr samples = NULL;
	try {
		TaskGenerator::Ptr taskGenerator = ownedPtr(new TaskGenerator(_td));
		taskGenerator->generateTask(100, _td->getInitState(), NULL, 0);
		
		targets = taskGenerator->getTasks();
		samples = taskGenerator->getSamples();
	} catch (...) {
		RW_THROW("Exception during grasp generation!");
	}
	
	/*
	 * Simulate grasping.
	 */
	try {
		GripperTaskSimulator::Ptr taskSimulator = ownedPtr(new GripperTaskSimulator(gripper, targets, samples, _td, 1));
		
		taskSimulator->startSimulation(_td->getInitState());
		
		while (taskSimulator->isRunning()) {
		}

		gripper->getQuality() = taskSimulator->getGripperQuality();
	} catch (...) {
		RW_THROW("Exception during grasp simulation!");
	}
	
	/*
	 * Extract results.
	 */
	GripperQuality& q = gripper->getQuality();
	results[0] = q.success;
	results[1] = q.robustness;
	results[2] = q.alignment;
	results[3] = q.coverage;
	results[4] = q.wrench;
	results[5] = -q.maxstress / _td->getStressLimit();
	results[6] = -1000.0 * q.volume;
	
	return results;
}


Gripper::Ptr GripperObjectiveFunction::parametersToGripper(const std::vector<double>& p) {
	Gripper::Ptr gripper = ownedPtr(new Gripper());
	
	/*
	 * Apply jaw parametrization.
	 */
	Q jawParameters(11);
	Q& j = jawParameters;
	j(0) = 0.0; // cut type
	j(1) = p[0]; // length
	j(2) = p[1]; // width
	j(3) = p[2]; // depth
	j(4) = p[3]; // chf. depth
	j(5) = p[4] * Deg2Rad; // chf.angle
	j(6) = p[8]; // cut position = tcp
	j(7) = p[5]; // cut depth
	j(8) = p[6] * Deg2Rad; // cut angle
	j(9) = 0.0; // cut radius
	j(10) = p[7] * Deg2Rad; // cut tilt
	
	gripper->setJawGeometry(jawParameters);
	
	/*
	 * Apply other parametrization.
	 */
	gripper->setForce(25.0);
	gripper->setJawdist(0.0);
	gripper->setOpening(0.05);
	gripper->setTCP(Transform3D<>(Vector3D<>(0.0, 0.0, p[8])));
	
	return gripper;
}
