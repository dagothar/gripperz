/**
 * @file RobustEvaluationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#include "RobustEvaluationManager.hpp"
#include <loaders/GripperXMLLoader.hpp>
#include <models/MapGripperBuilder.hpp>
#include <sstream>
#include <string>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::loaders;
using namespace gripperz::math;
using namespace std;
using namespace rw::common;


const int FailReturnCode = -1;
const int SuccessReturnCode = 0;
const int InterruptReturnCode = 2;


RobustEvaluationManager::RobustEvaluationManager(
	const std::string& dwcPath,
	const std::string& tdPath,
	const std::string& samplesPath,
	const Configuration& config,
	unsigned nCores,
	unsigned nTries,
	const string& tmpName,
	const string& executable
) :
	GripperEvaluationManager(config),
	_executable(executable),
	_cores(nCores),
	_dwcPath(dwcPath),
	_tdPath(tdPath),
	_samplesPath(samplesPath),
	_tmpName(tmpName),
	_ntries(nTries)
{
}


RobustEvaluationManager::~RobustEvaluationManager() {
}


GripperQuality::Ptr RobustEvaluationManager::evaluateGripper(Gripper::Ptr gripper) {
	Configuration config = getConfiguration();
	
	/*
	 * extract parameters
	 */
	GripperBuilder::Ptr builder = new MapGripperBuilder(gripper);
	Vector parameters = builder->gripperToParameters(gripper);
	string parameterString = toString(parameters, " ");
		
	/*
	 * construct command
	 * ${GRIPPERZ_ROOT}/bin/gripper_evaluate -c {cores} -g {simulations} -r {robustness} -v {values} --dwc {dwc} --td {td} --samples {samples} --name {name} 1>/dev/null 2>/dev/null"
	 */
	stringstream sstr;
	sstr << _executable;
	sstr << " -c " << _cores;
	sstr << " -g " << config.nOfGraspsPerEvaluation;
	sstr << " -r " << config.nOfRobustnessTargets;
	sstr << " -v " << parameterString;
	sstr << " --dwc " << _dwcPath;
	sstr << " --td " << _tdPath;
	if (!_samplesPath.empty()) sstr << " --samples " << _samplesPath;
	sstr << " --name " << _tmpName;
	
	string cmd = sstr.str();
	INFO << cmd << endl;
	
	/* execute */
	unsigned tries = 0;
	int returnCode = FailReturnCode;
	do {
		returnCode = system(cmd.c_str());
		
		if (returnCode == SuccessReturnCode || returnCode == InterruptReturnCode) {
			break;
		}
	} while (tries++ < _ntries);
	
	if (returnCode != SuccessReturnCode) {
		RW_WARN ("Evaluation failed");
		return ownedPtr(new GripperQuality);
	}
	
	/* read results */
	string filename = _tmpName + ".grp.xml";
	Gripper::Ptr evaluatedGripper = GripperXMLLoader::load(filename);
	
	/* return */
	GripperQuality::Ptr quality = ownedPtr(new GripperQuality(evaluatedGripper->getQuality()));
	return quality;
}
