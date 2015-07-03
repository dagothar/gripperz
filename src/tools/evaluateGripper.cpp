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
#include <simulator/GripperTaskSimulator.hpp>


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
using namespace rwsim::simulator;
using namespace boost::program_options;
namespace po = boost::program_options;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulator;
using namespace gripperz::grasps;
using namespace gripperz::loaders;


int main(int argc, char* argv[]) {
	Math::seed();
	RobWork::getInstance()->initialize();

	// options
	int cores, ntargets, nsamples, rtargets;
	string dwcFilename;
	string tdFilename;
	string gripperFilename;
	string outFilename;
	string samplesFilename;

	bool nosim = false;
	bool useSamples = false;
	bool testRobustness = false;

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
		("samples", value<string>(), "surface samples file")
		("out,o", value<string>(), "task file")
		("nosim", "don't perform simulation")
		("robustness,r", value<int>(&rtargets), "test robustnesss with s number of targets")
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

		if (vm.count("nosim")) {
			nosim = true;
		}

		if (vm.count("robustness")) {
			testRobustness = true;
		}
	} catch (...) {
		cout << usage << endl;
		cout << desc << endl;
		return -1;
	}

	/* load data */
	cout << "* Loading dwc... ";
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load(dwcFilename);
	cout << "Loaded." << endl;
	cout << "* Loading task description... ";
	TaskDescription::Ptr td = TaskDescriptionLoader::load(tdFilename, dwc);
	cout << "Loaded." << endl;
	cout << "* Loading gripper... ";
	Gripper::Ptr gripper = GripperXMLLoader::load(gripperFilename);
	cout << "Loaded." << endl;

	if (vm.count("out")) {
		outFilename = vm["out"].as<string>();
	} else {
		outFilename = gripper->getName() + ".tasks.xml";
	}

	vector<SurfaceSample> ssamples;
	if (vm.count("samples")) {
		samplesFilename = vm["samples"].as<string>();
		useSamples = true;

		// load samples
		ssamples = SurfaceSample::loadFromXML(samplesFilename);
	}
	
	rw::kinematics::State state = td->getInitState();

	/* update gripper */
	gripper->updateGripper(td->getWorkCell(), td->getDynamicWorkCell(),
			td->getGripperDevice(), td->getGripperDynamicDevice(),
			td->getInitState(), td);
	td->getInitState().upgrade();
	
	/* generate grasps */
	cout << "Generating grasps... nsamples=" << nsamples << endl;
	TaskGenerator::Ptr generator = new TaskGenerator(td);

	if (useSamples) {
		generator->generateTask(ntargets, td->getInitState(), &ssamples,
				nsamples);
	} else {
		generator->generateTask(ntargets, td->getInitState(), NULL, nsamples);
	}

	GraspTask::Ptr tasks = generator->getTasks();
	GraspTask::Ptr samples = generator->getSamples();
	cout << "Grasps generated." << endl;
	cout << "Tasks: " << tasks->getAllTargets().size() << endl;
	cout << "Samples: " << samples->getAllTargets().size()
			<< endl;

	/* perform simulation */
	//Log::log().setLevel(Log::Debug);
	if (!nosim) {
		cout << "Starting simulation..." << endl;
		GripperTaskSimulator::Ptr sim = ownedPtr(
				new GripperTaskSimulator(gripper, tasks, samples, td, cores));
		cout << "Simulator created." << endl;

		try {
			cout << "Launching..." << endl;
			sim->startSimulation(td->getInitState());
			cout << "Launched." << endl;
		} catch (...) {
			cout << "Error starting simulation..." << endl;
			return -1;
		}

		while (sim->isRunning()) {
			//cout << "Running..." << endl;
		}

		gripper->getQuality() = sim->getGripperQuality();
	}

	/* perform robustness tests */
	if (testRobustness) {
		cout << "Starting robustness test..." << endl;

		// perturbate only succesful tasks
		tasks = TaskGenerator::copyTasks(tasks, true);
		tasks = TaskGenerator::addPerturbations(tasks, sigma_p,
				sigma_a * Deg2Rad, rtargets);

		GripperTaskSimulator::Ptr sim = new GripperTaskSimulator(gripper, tasks,
				samples, td);
		sim->startSimulation(td->getInitState());

		while (sim->isRunning()) {
		}

		gripper->getQuality().robustness = sim->getGripperQuality().success;
	}

	/* display results */
	cout << "\nRESULTS" << endl;
	cout << gripper->getQuality() << endl;

	/* save results */
	GripperXMLLoader::save(gripper, gripperFilename);
	GraspTask::saveRWTask(tasks, outFilename);

	return 0;
}
