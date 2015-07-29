/**
 * @file gripper_evaluate.cpp
 * @author Adam Wolniakowski
 * @date 29-07-2015
 */

#include <iostream>
#include <vector>
#include <string>
#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <gripperz.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
using namespace robworksim;
USE_GRIPPERZ_NAMESPACE
using namespace gripperz;
using namespace boost::program_options;
namespace po = boost::program_options;


struct Configuration {
	int cores;
	int grasps;
	int robustness;
	vector<int> parameters;
	vector<double> values;
	vector<double> weights;
	string method;
	string dwc;
	string td;
	string samples;
	string name;
	double sigma_a;
	double sigma_p;
} configuration;


struct Data {
	DynamicWorkCell::Ptr dwc;
	TaskDescription::Ptr td;
	vector<SurfaceSample> samples;
} data;


void initialize() {
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Info);
}


bool parse_cli(int argc, char* argv[], Configuration& conf) {
	string usage =
		"This script evaluates gripper constructed from parameter values.\n\n"
		"Usage:\n"
		"\tevaluate-gripper";
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "help message")
		("cores,c", value<int>(&conf.cores)->default_value(1), "number of threads to use")
		("grasps,g", value<int>(&conf.grasps)->default_value(100), "number of grasps to perform")
		("robustness,r", value<int>(&conf.robustness)->default_value(100), "number of robustness tasks to generate")
		("parameters,p", value<vector<int> >(&conf.parameters)->multitoken(), "gripper parameter indices")
		("values,v", value<vector<double> >(&conf.values)->multitoken(), "gripper parameter values")
		("weights,w", value<vector<double> >(&conf.weights)->multitoken(), "7 weights for objectives (0-6)")
		("method,m", value<string>(&conf.method)->default_value("product"), "method for combining objectives (sum, product, log)")
		("dwc", value<string>(&conf.dwc)->required(), "dynamic workcell file")
		("td", value<string>(&conf.td)->required(), "task description file")
		("samples", value<string>(&conf.samples), "surface samples file")
		("name", value<string>(&conf.name), "gripper name")
		("sigma_a",	value<double>(&conf.sigma_a)->default_value(8), "standard deviation in of angle in degrees")
		("sigma_p",	value<double>(&conf.sigma_p)->default_value(0.003), "standard deviation of position in meters");
	variables_map vm;
	
	try {
		store(command_line_parser(argc, argv).options(desc).run(), vm);
		notify(vm);

		if (vm.count("help")) {
			cout << usage << endl;
			cout << desc << endl;
			return false;
		}

	} catch (exception& e) {
		cout << usage << endl;
		cout << desc << endl;
		return false;
	}
	
	return true;
}


GripperObjectiveFunction::Ptr make_objective_function(const Configuration& config, const Data& data) {
	/* evaluation manager */
	GripperEvaluationManager::Configuration configuration;
	configuration.nOfGraspsPerEvaluation = config.grasps;
	configuration.nOfRobustnessTargets = config.robustness;
	configuration.sigma_a = config.sigma_a;
	configuration.sigma_p = config.sigma_p;
	GripperEvaluationManager::Ptr manager = GripperEvaluationManagerFactory::getEvaluationManager(data.td, configuration, config.cores, data.samples);
	
	/* gripper builder */
	vector<MapGripperBuilder::ParameterName> parameters;
	vector<string> names;
	BOOST_FOREACH (int id, config.parameters) {
		MapGripperBuilder::ParameterName p = static_cast<MapGripperBuilder::ParameterName>(id);
		parameters.push_back(p);
		names.push_back(MapGripperBuilder::parameterNameToString(p));
	}
	GripperBuilder::Ptr builder = new MapGripperBuilder(new Gripper(), parameters);
	
	/* objective */
	GripperObjectiveFunction::Ptr objective = new GripperObjectiveFunction(builder, manager);
	
	return objective;
}


void load_data(const Configuration& config, Data& data) {
	INFO << "* Loading dwc... ";
	data.dwc = DynamicWorkCellLoader::load(config.dwc);
	INFO << "Loaded." << endl;
	INFO << "* Loading task description... " << config.td << " " << endl;
	data.td = TaskDescriptionLoader::load(config.td, data.dwc);
	INFO << "Loaded." << endl;

	if (!config.samples.empty()) {
		INFO << "* Loading samples... ";
		data.samples = SurfaceSample::loadFromXML(config.samples);
		INFO << "Loaded." << endl;
	}
}


int main(int argc, char* argv[]) {
	initialize();
	
	if (!parse_cli(argc, argv, configuration)) {
		return 0;
	}
	
	try {
		load_data(configuration, data);
	} catch (exception& e) {
		cout << "Exception during loading data: " << e.what() << endl;
		return 0;
	}
	
	GripperObjectiveFunction::Ptr objective = make_objective_function(configuration, data);
	
	return 0;
}
