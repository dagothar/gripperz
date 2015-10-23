/**
 * @file objective_example.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-22
 * 
 * @note This script is meant to run from bin directory!
 */

#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
using namespace robworksim;


int main(int argc, char* argv[]) {
	/* 
	 * Initialize RobWork.
	 */
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Info);
	
	/*
	 * Load data needed for gripper evaluation:
	 * DWC, task description file, base gripper file, surface samples.
	 * 
	 * In order to evaluate different scene, paths should be changed accordingly.
	 * Each scene needs corresponding td, dwc, and samples file.
	 * They are all in the same directory.
	 * Tasks are numbered 1 to 3. Task and sample file belong to a scene.
	 * Numbers should be the same, i.e. task1.td.xml <-> samples1.xml.
	 */
	INFO << "* Loading workcell... ";
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load("../data/rotor/Scene.dwc.xml");
	INFO << "Loaded." << endl;
	INFO << "* Loading context... " ;
	TaskDescription::Ptr td = TaskDescriptionLoader::load("../data/rotor/task1.td.xml", dwc);
	INFO << "Loaded." << endl;
	INFO << "* Loading samples... ";
	vector<SurfaceSample> ssamples = SurfaceSample::loadFromXML("../data/rotor/samples1.xml");
	INFO << "Loaded." << endl;
	INFO << "* Loading gripper... ";
	OldGripper::Ptr gripper = GripperXMLLoader::load("../data/grippers/template.grp.xml");
	INFO << "Loaded." << endl;
	
	/*
	 * Build evaluation manager.
	 * Manager takes care of setting up task generation, simulation, etc.
	 * If calculation of robustness index is neccesary, nOfRobustnessTargets should be set to the value of nOfGraspsPerEvaluation.
	 */
	GripperEvaluationProcessManager::Configuration config;
	config.nOfGraspsPerEvaluation = 100;		// number of simulations per gripper evaluation
	config.nOfRobustnessTargets = 0;			// robustness targets
	
	GripperEvaluationProcessManager::Ptr manager = GripperEvaluationManagerFactory::makeStandardEvaluationManager(
		td,			// task description
		config,		// configuration
		4,			// number of threads to use
		ssamples	// surface samples vector
	);
	
	/* 
	 * Build objective function.
	 * 
	 * The objective function uses gripper builder and the evaluation manager.
	 * Gripper builder takes a gripper template, and depending on configured parameters modifies it to be
	 * evaluated by the manager.
	 * Thus, the objective takes N parameters (in vector<double> format). It returns 7 results (as a vector<double>).
	 */
	vector<MapGripperBuilder::ParameterName> params{ // this is the order of parameters!
		MapGripperBuilder::Length,
		MapGripperBuilder::Width,
		MapGripperBuilder::Depth,
		MapGripperBuilder::ChfDepth,
		MapGripperBuilder::ChfAngle,
		MapGripperBuilder::CutDepth,
		MapGripperBuilder::CutAngle,
		MapGripperBuilder::CutTilt, 
		MapGripperBuilder::TcpOffset,
		MapGripperBuilder::Jawdist,
		MapGripperBuilder::Stroke,
		MapGripperBuilder::Force
	};
	GripperBuilder::Ptr builder = new MapGripperBuilder(gripper, params);
	
	MultiObjectiveFunction::Ptr objective = new GripperObjectiveFunction(builder, manager); // takes 12 parameters, returns 7 objectives
	
	/*
	 * This is how to use the objective function.
	 */
	vector<double> parameters{0.1, 0.025, 0.02, 1.0, 45.0, 0.01, 120.0, 0.0, 0.025, 0.0, 0.05, 25.0};
	vector<double> results = objective->evaluate(parameters);
	
	// display results
	INFO << "success= " << results[0] << endl;
	INFO << "robustness= " << results[1] << endl;
	INFO << "alignment= " << results[2] << endl;
	INFO << "coverage= " << results[3] << endl;
	INFO << "wrench= " << results[4] << endl;
	INFO << "stress= " << results[5] << endl;
	INFO << "volume= " << results[6] << endl;
	
	/*
	 * Output should look like:
	 * success= 0.771739
	 * robustness= 1
	 * alignment= 0.113461
	 * coverage= 0.284768
	 * wrench= 0.0900634
	 * stress= 0.7
	 * volume= 0.597141
	 * Combined quality is: 0.369406
	 * 
	 * That can happen:
	 * Error stepping Id[-1]ODESimulator.cpp:633 : Too Large Penetrations!
	 */
	
	/*
	 * You can also use utilities like these:
	 */
	vector<double> weights{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	CombineObjectives::Ptr combiningMethod = CombineObjectivesFactory::make("product", weights); // this makes the objective combining method (accepts: sum / product / log)
	
	// now use:
	double Q = combiningMethod->combine(results);
	INFO << "Combined quality is: " << Q << endl;
	
	// or make a single objective function:
	ObjectiveFunction::Ptr singleObjective = new CombinedFunction(objective, combiningMethod);
	// then evaluate:
	// double Q = singleObjective->evaluate(parameters);
	
	/*
	 * Use boost::bind to make a function:
	 */
	boost::function<vector<double>(const vector<double>&)> boostFunction =
		boost::bind(&MultiObjectiveFunction::evaluate, objective.get(), _1);
	
	return 0;
}
