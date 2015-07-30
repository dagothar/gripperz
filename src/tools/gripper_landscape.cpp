/**
 * @file gripper_landscape.cpp
 * @author Adam Wolniakowski
 * @date 7-07-2015
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
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
#include <models/MapGripperBuilder.hpp>
#include <evaluation/GripperObjectiveFunction.hpp>
#include <evaluation/GripperEvaluationManager.hpp>
#include <evaluation/RobustEvaluationManager.hpp>
#include <evaluation/GripperEvaluationManagerFactory.hpp>
#include <math/CombineObjectivesFactory.hpp>


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
using namespace gripperz::math;
using namespace gripperz::evaluation;
using namespace gripperz::grasps;
using namespace gripperz::loaders;


string vectorToString(const vector<double>& v) {
	stringstream sstr;
	
	for (unsigned i = 0; i < v.size(); ++i) {
		sstr << v[i] << ", ";
	}
	
	return sstr.str();
}


int main(int argc, char* argv[]) {
	/* initialize robwork */
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Info);
	
	/* parameter bounds */
	map<MapGripperBuilder::ParameterName, pair<double, double> > bounds;
	bounds[MapGripperBuilder::Length] = make_pair(0.0, 0.2);
	bounds[MapGripperBuilder::Width] = make_pair(0.0, 0.05);
	bounds[MapGripperBuilder::Depth] = make_pair(0.0, 0.05);
	bounds[MapGripperBuilder::ChfDepth] = make_pair(0.0, 1.0);
	bounds[MapGripperBuilder::ChfAngle] = make_pair(0.0, 90.0);
	bounds[MapGripperBuilder::CutDepth] = make_pair(0.0, 0.05);
	bounds[MapGripperBuilder::CutAngle] = make_pair(0.0, 180.0);
	bounds[MapGripperBuilder::CutTilt] = make_pair(-90.0, 90.0);
	bounds[MapGripperBuilder::TcpOffset] = make_pair(0.0, 0.2);
	bounds[MapGripperBuilder::Jawdist] = make_pair(0.0, 0.05);
	bounds[MapGripperBuilder::Stroke] = make_pair(0.0, 0.05);
	bounds[MapGripperBuilder::Force] = make_pair(0.0, 50.0);

	/* options */
	int cores, ntargets, nrobust, resolution;
	string dwcFilename;
	string tdFilename;
	string gripperFilename;
	string outDir;
	string samplesFilename;
	vector<int> parameters{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	vector<double> weights{1, 1, 1, 1, 1, 1, 1};

	/* define CLI options */
	string usage =
			"This is a script used to generate tasks for a single gripper, simulate them and"
					" evaluate gripper's performance.\n\n"
					"Usage:\n"
					"evaluate-gripper";
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "help message")
		("cores,c", value<int>(&cores)->default_value(1), "number of threads to use")
		("res", value<int>(&resolution)->default_value(10), "how many samples per parameter")
		("ntargets,t", value<int>(&ntargets)->default_value(100), "number of tasks to generate")
		("nrobust,r", value<int>(&nrobust)->default_value(100), "number of robustness tasks to generate")
		("parameters,p", value<vector<int> >(&parameters)->multitoken(), "9 parameters to landscape (0-8)")
		("weights,w", value<vector<double> >(&weights)->multitoken(), "7 weights for objectives (0-6)")
		("dwc", value<string>(&dwcFilename)->required(), "dynamic workcell file")
		("td", value<string>(&tdFilename)->required(), "task description file")
		("gripper,g", value<string>(&gripperFilename)->required(), "gripper file")
		("ssamples,s", value<string>(&samplesFilename)->default_value(""), "surface samples file")
		("out,o", value<string>(&outDir)->required(), "output directory");
	variables_map vm;
	
	/* parse CLI */
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
	INFO << "* Loading gripper... ";
	Gripper::Ptr gripper = GripperXMLLoader::load(gripperFilename);
	INFO << "Loaded." << endl;
	
	/* construct objective function */
	GripperEvaluationManager::Configuration config;
	config.nOfGraspsPerEvaluation = ntargets;
	config.nOfRobustnessTargets = nrobust;
	GripperEvaluationManager::Ptr manager = new RobustEvaluationManager(
		dwcFilename,
		tdFilename,
		samplesFilename,
		config,
		cores
	);
	
	CombineObjectives::Ptr sumMethod = CombineObjectivesFactory::make("sum", weights);
	CombineObjectives::Ptr logMethod = CombineObjectivesFactory::make("log", weights);
	
	/* landscapes */
	BOOST_FOREACH (int id, parameters) {
		MapGripperBuilder::ParameterName name = static_cast<MapGripperBuilder::ParameterName>(id);
		
		string paramName = MapGripperBuilder::parameterNameToString(name);
		ofstream dataFile(outDir + "/" + paramName + ".csv");
		dataFile << "# " << paramName << ", success, robustness, alignment, coverage, wrench, stress, volume, qsum, qlog" << endl;
		
		GripperBuilder::Ptr builder = new MapGripperBuilder(gripper, vector<MapGripperBuilder::ParameterName>{name});
		GripperObjectiveFunction::Ptr func = new GripperObjectiveFunction(builder, manager);
		
		double range = bounds[name].second - bounds[name].first;
		double step = range / resolution;
		int n = 0;
		for (double x = bounds[name].first; x < bounds[name].second + step; x+= step) {
			cout << "# Evaluating " << paramName << "= " << x << endl;
			vector<double> param{x};
			vector<double> result = func->evaluate(param);
			double q_sum = sumMethod->combine(result);
			double q_log = logMethod->combine(result);
			
			cout << x << ", " << vectorToString(result) << q_sum << ", " << q_log << endl;
			dataFile << x << ", " << vectorToString(result) << q_sum << ", " << q_log << endl;
			
			Gripper::Ptr grp = func->getLastGripper();
			stringstream sstr;
			sstr << outDir << "/" << paramName << "_" << n++ << ".grp.xml";
			GripperXMLLoader::save(grp, sstr.str());
		}
		
		dataFile.close();
	}

	return 0;
}
