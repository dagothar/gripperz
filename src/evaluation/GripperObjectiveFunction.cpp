/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "GripperObjectiveFunction.hpp"

#include <rwlibs/task/GraspTask.hpp>
#include <grasps/TaskGenerator.hpp>
#include <simulation/GripperTaskSimulator.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;


GripperObjectiveFunction::GripperObjectiveFunction(GripperBuilder::Ptr builder, evaluation::GripperEvaluationManager::Ptr manager) :
	_builder(builder),
	_manager(manager)
{
}


GripperObjectiveFunction::~GripperObjectiveFunction() {
}


bool areParametersOk(const std::vector<double>& p) {
	// if tcp > length
	if (p[8] > p[0]) {
		DEBUG << "tcp > length" << endl;
		return false;
	}
	
	// if cut depth > width
	if (p[5] > p[2]) {
		DEBUG << "cut depth > width" << endl;
		return false;
	}
	
	// check if cutout doesn't fall under chamfer
	double chflength = p[3] * p[1] * sin(p[4] * Deg2Rad);
	if (p[8] > p[0] - chflength) {
		double y = p[1] - (p[8] - chflength) * cos(p[4] * Deg2Rad);
		if (p[5] > y) {
			DEBUG << "too thin under cutout" << endl;
			return false;
		}
	}
	
	return true;
}


std::vector<double> GripperObjectiveFunction::operator()(const std::vector<double>& x) {
	vector<double> results(NObjectives, 0.0);
	
	//if (!areParametersOk(x)) {
	//	return results;
	//}
	
	/*
	 * Build gripper.
	 */
	Gripper::Ptr gripper = NULL;
	try {
		gripper = _builder->parametersToGripper(x);
	} catch (...) {
		RW_THROW("Exception during gripper generation!");
	}
	
	try {
		GripperQuality::Ptr q = _manager->evaluateGripper(gripper);
		
		/*
		 * Extract results.
		 */
		results[0] = q->success;
		results[1] = q->robustness;
		results[2] = q->alignment;
		results[3] = q->coverage;
		results[4] = q->wrench;
		results[5] = q->maxstress;
		results[6] = q->volume;
	} catch (...) {
		INFO << "Exception during gripper evaluation!" << endl;
	}
	
	
	
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
