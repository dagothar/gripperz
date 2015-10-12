/**
 * @file gripper_optimizer.cpp
 * @author Adam Wolniakowski
 * @date 20-07-2015
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <gripperz.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/filesystem.hpp>


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
using namespace boost::filesystem;
namespace po = boost::program_options;


struct {
	int cores;
	string dwc_filename;
	string td_filename;
	string ssamples_filename;
	string gripper_filename;
	string out_dir;
	int n_targets;
	int n_robust;
	string combiner;
	string optimizer;
	vector<double> weights;
	vector<int> parameters;
	double sigma_a, sigma_p;
	double rho0, rhof;
	int maxfev;
} Configuration;


RangeList ranges{
	{0, 0.2},
	{0, 0.05},
	{0, 0.05},
	{0, 1},
	{0, 90},
	{0, 0.05},
	{0, 180},
	{-90, 90},
	{0, 0.2},
	{0, 0.05},
	{0, 0.05},
	{0, 50}
};


vector<vector<double> > opt_log;
ofstream log_file;
path outdir;

void callback(OldGripper::Ptr gripper, CombineObjectives::Ptr combiner, GripperBuilder::Ptr builder) {
	static unsigned step = 0;
	
	Vector entry;
	entry.push_back(step);
	
	Vector args = builder->gripperToParameters(gripper);
	
	OldGripperQuality::Ptr q = &gripper->getQuality();
	Vector res{
		q->success,
		q->robustness,
		q->alignment,
		q->coverage,
		q->wrench,
		q->stress,
		q->volume
	};
	
	entry.insert(entry.end(), args.begin(), args.end());
	entry.insert(entry.end(), res.begin(), res.end());
	
	double quality = combiner->combine(res);
	entry.push_back(quality);
	
	opt_log.push_back(entry);
	
	++step;
	
	for (unsigned i = 0; i < entry.size(); ++i) {
		cout << entry[i];
		log_file << entry[i];
		if (i < entry.size() - 1) {
			cout << ", ";
			log_file << ", ";
		}
	}
	cout << endl;
	log_file << endl;
	
	stringstream sstr;
	sstr << "step_" << step << ".grp.xml";
	path gripper_file = outdir / path(sstr.str());
	GripperXMLLoader::save(gripper,gripper_file.string());
}


int main(int argc, char* argv[]) {
	
	/* initialize robwork */
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Info);


	/* options */
	string usage =
		"This is a script used optimize grippers.\n\n"
		"Usage:\n"
		"gripper_optimizer";
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "help message")
		("cores,c", value<int>(&Configuration.cores)->default_value(1), "number of threads to use")
		("ntargets,t", value<int>(&Configuration.n_targets)->default_value(100), "number of tasks to generate")
		("nrobust,r", value<int>(&Configuration.n_robust)->default_value(100), "number of robustness tasks to generate")
		("parameters,p", value<vector<int> >(&Configuration.parameters)->multitoken(), "12 parameters to optimize (0-11)")
		("weights,w", value<vector<double> >(&Configuration.weights)->multitoken(), "7 weights for objectives (0-6)")
		("dwc", value<string>(&Configuration.dwc_filename)->required(), "dynamic workcell file")
		("td", value<string>(&Configuration.td_filename)->required(), "task description file")
		("gripper,g", value<string>(&Configuration.gripper_filename)->required(), "gripper file")
		("out,o", value<string>(&Configuration.out_dir)->required(), "output directory")
		("ssamples,s", value<string>(&Configuration.ssamples_filename), "surface samples file")
		("combiner", value<string>(&Configuration.combiner)->default_value("product"), "objective combining method")
		("optimizer", value<string>(&Configuration.optimizer)->default_value("simplex"), "optimization method (simplex, coord, bobyqa, simu, powell)")
		("sigma_a",	value<double>(&Configuration.sigma_a)->default_value(8), "standard deviation in of angle in degrees")
		("sigma_p",	value<double>(&Configuration.sigma_p)->default_value(0.003), "standard deviation of position in meters")
		("rho0",	value<double>(&Configuration.rho0)->default_value(0.1), "initial step size")
		("rhof",	value<double>(&Configuration.rhof)->default_value(0.001), "final step size")
		("maxfev",	value<int>(&Configuration.maxfev)->default_value(1e2), "max number of function evaluations");
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
	INFO << "* Loading dwc... " << Configuration.dwc_filename;
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load(Configuration.dwc_filename);
	INFO << " Loaded." << endl;
	INFO << "* Loading task description... " << Configuration.td_filename;
	TaskDescription::Ptr td = TaskDescriptionLoader::load(Configuration.td_filename, dwc);
	INFO << " Loaded." << endl;
	INFO << "* Loading gripper... " << Configuration.gripper_filename;
	OldGripper::Ptr gripper = GripperXMLLoader::load(Configuration.gripper_filename);
	INFO << " Loaded." << endl;

	vector<SurfaceSample> ssamples;
	if (vm.count("samples")) {
		INFO << "* Loading samples... " << Configuration.ssamples_filename;
		ssamples = SurfaceSample::loadFromXML(Configuration.ssamples_filename);
		INFO << " Loaded." << endl;
	}
	
	outdir = path(Configuration.out_dir);
	if (!exists(outdir)) {
		if (!create_directory(outdir)) {
			RW_THROW ("Unable to create directory: " << Configuration.out_dir);
		}
	}
	
	
	/* construct evaluation manager */
	GripperEvaluationManager::Configuration config;
	config.nOfGraspsPerEvaluation = Configuration.n_targets;
	config.nOfRobustnessTargets = Configuration.n_robust;
	config.sigma_a = Configuration.sigma_a;
	config.sigma_p = Configuration.sigma_p;
	GripperEvaluationManager::Ptr manager = GripperEvaluationManagerFactory::makeStandardEvaluationManager(td, config, Configuration.cores, ssamples);
	
	
	/* construct objective function */
	vector<MapGripperBuilder::ParameterName> params;
	RangeList opt_ranges;
	BOOST_FOREACH (int id, Configuration.parameters) {
		params.push_back(static_cast<MapGripperBuilder::ParameterName>(id));
		opt_ranges.push_back(ranges[id]);
	}
	GripperBuilder::Ptr builder = new MapGripperBuilder(gripper, params);
	GripperObjectiveFunction::Ptr multi_function = new GripperObjectiveFunction(builder, manager);
	
	CombineObjectives::Ptr comb_method = CombineObjectivesFactory::make(Configuration.combiner, Configuration.weights);
	ObjectiveFunction::Ptr objective = new CombinedFunction(multi_function, comb_method);
	
	log_file.open((outdir / path("log.csv")).string());
	multi_function->setCallback(boost::bind<>(&callback, _1, comb_method, new MapGripperBuilder(gripper)));
	
	
	/* construct optimization manager */
	Optimizer::Ptr optimizer;
	if (Configuration.optimizer == "simplex") {
		optimizer = OptimizerFactory::makeSimplexOptimizer(Configuration.rho0, Configuration.rhof, Configuration.maxfev);
	}
	
	if (Configuration.optimizer == "coord") {
		optimizer = OptimizerFactory::makeCoordinateDescentOptimizer(Configuration.rho0, Configuration.rhof, Configuration.maxfev);
	}
	
	if (Configuration.optimizer == "bobyqa") {
		optimizer = OptimizerFactory::makeBOBYQAOptimizer(params.size(), Configuration.rho0, Configuration.rhof, Configuration.maxfev);
	}
	
	if (Configuration.optimizer == "simu") {
		optimizer = OptimizerFactory::makeSimulatedAnnealingOptimizer(Configuration.rho0, Configuration.rhof, Configuration.maxfev);
	}
	
	if (Configuration.optimizer == "powell") {
		optimizer = OptimizerFactory::makePowellOptimizer(Configuration.rho0, Configuration.rhof, Configuration.maxfev);
	}
	
	if (!optimizer) {
		RW_THROW ("Optimizer type not supported!");
	}

	OptimizationManager::Ptr opt_manager = new OptimizationManager(optimizer, opt_ranges, true);
	
	
	/* perform optimization */
	cout << "#step, length, width, depth, chfdepth, chfangle, cut depth, cut angle, tilt, tcp, jawdist, stroke, force, success, robustness, alignment, coverage, wrench, stress, volume, q" << endl;
	log_file << "#step, length, width, depth, chfdepth, chfangle, cut depth, cut angle, tilt, tcp, jawdist, stroke, force, success, robustness, alignment, coverage, wrench, stress, volume, q" << endl;

	Vector initialGuess = builder->gripperToParameters(gripper);
	Vector result = opt_manager->optimize(objective, initialGuess, "maximize");
	
	OldGripper::Ptr opt_gripper = builder->parametersToGripper(result);
	OldGripperQuality::Ptr opt_gripper_q = manager->evaluateGripper(opt_gripper);
	gripper->setQuality(*opt_gripper_q);
	
	cout << "Optimization succesful!" << endl;
	cout << "Result: " << *opt_gripper << endl;
	cout << "Quality: " << *opt_gripper_q << endl;
	
	/* save results */
	path opt_gripper_file = outdir / path("result.grp.xml");
	GripperXMLLoader::save(opt_gripper, opt_gripper_file.string());

	log_file.close();
	
	return 0;
}

