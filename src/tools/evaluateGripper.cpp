/**
 * @file evaluateGripper.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include <iostream>
#include <rw/rw.hpp>
#include <rw/RobWork.hpp>
#include <rw/common/Log.hpp>
#include <rw/common/ExtensionRegistry.hpp>
#include <rwsim/rwsim.hpp>
#include <rwsim/loaders/DynamicWorkCellLoader.hpp>
#include <rw/loaders/model3d/STLFile.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>
#include <rwsimlibs/ode/ODEPlugin.hpp>

#include <context/TaskDescription.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include <models/Gripper.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <grasps/TaskGenerator.hpp>
#include <simulation/GripperTaskSimulator.hpp>
#include <simulation/InterferenceSimulator.hpp>
#include <evaluation/GripperEvaluator.hpp>
#include <evaluation/GripperEvaluationManager.hpp>
#include <evaluation/GripperEvaluationManagerFactory.hpp>


#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
using namespace rw::models;
using namespace rw::common;
using namespace rw::loaders;
using namespace rwlibs::task;
using namespace rwsim;
using namespace rwsim::dynamics;
using namespace rwsim::loaders;
using namespace rwsim::simulation;
using namespace boost::program_options;
namespace po = boost::program_options;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::evaluation;
using namespace gripperz::grasps;
using namespace gripperz::loaders;


int main(int argc, char* argv[]) {
	/* initialize RW */
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Info);

	// options
	int cores, ntargets, nsamples, rtargets;
	string dwcFilename;
	string tdFilename;
	string gripperFilename;
	string outFilename;
	string samplesFilename;

	double sigma_a, sigma_p;

	// program options
	string usage =
			"This is a script used to generate tasks for a single gripper, simulate them and"
					" evaluate gripper's performance.\n\n"
					"Usage:\n"
					"evaluate-gripper";
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "help message")
		("cores,c", value<int>(&cores)->default_value(1), "number of threads to use")
		("ntargets,t", value<int>(&ntargets)->default_value(0), "number of tasks to generate")
		("nsamples,s", value<int>(&nsamples)->default_value(0), "number of samples to use")
		("dwc", value<string>(&dwcFilename)->required(), "dynamic workcell file")
		("td", value<string>(&tdFilename)->required(), "task description file")
		("gripper,g", value<string>(&gripperFilename)->required(), "gripper file")
		("ssamples", value<string>(), "surface samples file")
		//("out,o", value<string>(), "task file")
		("nrobust,r", value<int>(&rtargets)->default_value(100), "test robustnesss with s number of targets")
		("sigma_a",	value<double>(&sigma_a)->default_value(8), "Standard deviation in of angle in degrees.")
		("sigma_p",	value<double>(&sigma_p)->default_value(0.003), "Standard deviation of position in meters.");
	variables_map vm;
	try {
		store(command_line_parser(argc, argv).options(desc).run(), vm);
		notify(vm);

		if (vm.count("help")) {
			cout << usage << endl;
			cout << desc << endl;
			return 0;
		}
	} catch (...) {
		cout << usage << endl;
		cout << desc << endl;
		return -1;
	}

	/* load data */
	INFO << "* Loading dwc... ";
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load(dwcFilename);
	INFO << "Loaded." << endl;
	INFO << "* Loading task description... " << tdFilename << " " << endl;
	TaskDescription::Ptr td = TaskDescriptionLoader::load(tdFilename, dwc);
	INFO << "Loaded." << endl;
	INFO << "* Loading gripper... ";
	Gripper::Ptr gripper = GripperXMLLoader::load(gripperFilename);
	INFO << "Loaded." << endl;

	vector<SurfaceSample> ssamples;
	if (vm.count("samples")) {
		samplesFilename = vm["ssamples"].as<string>();

		// load samples
		ssamples = SurfaceSample::loadFromXML(samplesFilename);
	}
	
	/* construct evaluation manager */
	GripperEvaluationManager::Configuration config;
	config.nOfGraspsPerEvaluation = ntargets;
	config.nOfRobustnessTargets = rtargets;
	config.sigma_a = sigma_a;
	config.sigma_p = sigma_p;
	GripperEvaluationManager::Ptr manager = GripperEvaluationManagerFactory::getEvaluationManager(td, config, cores, ssamples);
	
	/* evaluate gripper */
	GripperQuality::Ptr quality = manager->evaluateGripper(gripper);

	gripper->setQuality(*quality);

	/* display results */
	INFO << "\nRESULTS" << endl;
	INFO << gripper->getQuality() << endl;

	/* save results */
	GripperXMLLoader::save(gripper, gripperFilename);
	//GraspTask::saveRWTask(tasks, outFilename);

	return 0;
}
