/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "GripperObjectiveFunction.hpp"

#include <rwlibs/task/GraspTask.hpp>
#include <grasps/GraspSource.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::process;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace gripperz::math;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;


GripperObjectiveFunction::GripperObjectiveFunction(GripperBuilder::Ptr builder, GripperEvaluationManager::Ptr manager, GripperQualityExtractor::Ptr extractor) :
	_builder(builder),
	_manager(manager),
	_extractor(extractor)
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


std::vector<double> GripperObjectiveFunction::evaluate(const std::vector<double>& x) {
	Vector results(NObjectives, 0.0);
	
	/*
	 * Build gripper.
	 */
	OldGripper::Ptr gripper = NULL;
	try {
		
		gripper = _builder->parametersToGripper(x);
		_lastGripper = gripper;
		
	} catch (const std::exception& e) {
		RW_THROW("Exception during gripper generation! " << e.what());
	}
	
	/*
	 * Evaluate gripper.
	 */
	
	try {
		
		OldGripperQuality::Ptr q = _manager->evaluateGripper(gripper);
		
		_lastQuality = q;
		_lastGripper->setQuality(*q);
		
		results = _extractor->extract(q);
	} catch (const std::exception& e) {
		RW_THROW ("Exception during gripper evaluation! " << e.what());
	}
	
	/*
	 * Callback (if exists).
	 */
	if (_callback) {
		_callback(_lastGripper);
	}
	
	return results;
}
