/**
 * @file gripper_opt.cpp
 * @author Adam Wolniakowski
 * @date 9-07-2015
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
#include <evaluation/GripperEvaluationManagerFactory.hpp>
#include <optimization/CombineObjectivesFactory.hpp>
#include <optimization/RevertedFunction.hpp>
#include <optimization/CombinedFunction.hpp>
#include <optimization/BOBYQAOptimizer.hpp>


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
using namespace gripperz::optimization;
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

	/* options */
	int cores, ntargets, nrobust, maxfev;
	string dwcFilename;
	string tdFilename;
	string gripperFilename;
	string outDir;
	string samplesFilename;
	vector<int> parameters{0, 1, 2, 3, 4, 5, 6, 7, 8};
	vector<double> weights{1, 1, 1, 1, 1, 1, 1};
	
	BOBYQAOptimizer::ConstraintList constraints{
		{0, 0.2}, {0, 0.05}, {0, 0.05}, {0, 1}, {0, 45}, {0, 0.05}, {0, 180}, {-90, 90}, {0, 0.2}
	};

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
		("ntargets,t", value<int>(&ntargets)->default_value(100), "number of tasks to generate")
		("nrobust,r", value<int>(&nrobust)->default_value(0), "number of robustness tasks to generate")
		("maxfev,m", value<int>(&maxfev)->default_value(1000), "max number of function evaluations")
		("parameters,p", value<vector<int> >(&parameters)->multitoken(), "parameters to optimize")
		("weights,w", value<vector<double> >(&weights)->multitoken(), "7 weights for objectives (0-6)")
		("dwc", value<string>(&dwcFilename)->required(), "dynamic workcell file")
		("td", value<string>(&tdFilename)->required(), "task description file")
		("gripper,g", value<string>(&gripperFilename)->required(), "initial gripper design")
		("ssamples,s", value<string>(&samplesFilename), "surface samples file")
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
		INFO << "* Loading samples... ";
		ssamples = SurfaceSample::loadFromXML(samplesFilename);
		INFO << "Loaded." << endl;
	}
	
	/* set up objective fuction */
	GripperEvaluationManager::Configuration config;
	config.nOfGraspsPerEvaluation = ntargets;
	config.nOfRobustnessTargets = nrobust;
	GripperEvaluationManager::Ptr manager = GripperEvaluationManagerFactory::getEvaluationManager(td, config, cores, ssamples);

	vector<MapGripperBuilder::ParameterName> params;
	BOOST_FOREACH (int id, parameters) {
		params.push_back(static_cast<MapGripperBuilder::ParameterName>(id));
	}
	GripperBuilder::Ptr builder = new MapGripperBuilder(gripper, params);
	MultiObjectiveFunction::Ptr func = new GripperObjectiveFunction(builder, manager);
	
	CombineObjectives::Ptr logMethod = CombineObjectivesFactory::make("log", weights);
	ObjectiveFunction::Ptr objective = new RevertedFunction(new CombinedFunction(func, logMethod));
	
	/* perform optimization */
	BOBYQAOptimizer::ConstraintList constr;
	BOOST_FOREACH (int p, parameters) {
		constr.push_back(constraints[p]);
	}
	BOBYQAOptimizer::Configuration opt_config;
	opt_config.initialTrustRegionRadius = 0.01;
	opt_config.finalTrustRegionRadius = 0.001;

	Optimizer::Ptr optimizer = new BOBYQAOptimizer(opt_config, constr);
	
	/* perform optimization */
	vector<double> initialGuess = builder->gripperToParameters(gripper);
	vector<double> result = optimizer->minimize(objective, initialGuess);
	
	/* print result */
	cout << "Result." << endl;
	Gripper::Ptr opt_gripper = builder->parametersToGripper(result);
	GripperXMLLoader::save(opt_gripper, "optGripper.grp.xml");
	
	return 0;
}

