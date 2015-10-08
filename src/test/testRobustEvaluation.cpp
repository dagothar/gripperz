/**
 * @file testRobustEvaluation.hpp
 * @author Adam Wolniakowski
 * @date 30-07-2015
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <evaluation/GripperObjectiveFunction.hpp>
#include <evaluation/GripperEvaluationManager.hpp>
#include <evaluation/RobustEvaluationManager.hpp>
#include <evaluation/GripperEvaluationManagerFactory.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <simulation/InterferenceSimulator.hpp>
#include <math/CombineObjectivesFactory.hpp>
#include <models/MapGripperBuilder.hpp>


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::math;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace gripperz::loaders;
using namespace gripperz::models;
USE_ROBWORK_NAMESPACE
using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
using namespace robworksim;


vector<double> readVector(istream& stream) {
	vector<double> v;
	
	string str;
	getline(stream, str);
	stringstream sstr(str);
	
	double d;
	while (sstr >> d) {
		v.push_back(d);
	}
	
	return v;
}


string vectorToString(const vector<double>& v) {
	stringstream sstr;
	
	for (unsigned i = 0; i < v.size(); ++i) {
		sstr << v[i] << " ";
	}
	
	return sstr.str();
}


int main(int argc, char* argv[]) {
	/* initialize RW */
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Info);
	
	vector<double> weights{1, 1, 1, 1, 1, 1, 1};
	cout << "Weights: " << vectorToString(weights) << endl;
	
	/* create objective function */
	vector<MapGripperBuilder::ParameterName> params{
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
	GripperBuilder::Ptr builder = new MapGripperBuilder(new Gripper, params);
	
	GripperEvaluationManager::Configuration config;
	GripperEvaluationManager::Ptr manager = new RobustEvaluationManager(
		"../data/rotor/Scene.dwc.xml",
		"../data/rotor/task1.td.xml",
		"../data/rotor/samples1.xml",
		config,
		4 // cores
	);
	MultiObjectiveFunction::Ptr func = new GripperObjectiveFunction(builder, manager);
	
	/* initialize combiners */
	CombineObjectives::Ptr sumMethod = CombineObjectivesFactory::make("sum", weights);
	CombineObjectives::Ptr logMethod = CombineObjectivesFactory::make("log", weights);
	
	/* read grippers */
	vector<double> param{0.1, 0.025, 0.02, 0, 0, 0, 0, 0, 0.025, 0, 0.05, 25};
		
	/* evaluate */
	vector<double> result = func->evaluate(param);
	
	cout << "Objectives (success, robustness, alignment, coverage, wrench, stress, volume): " << vectorToString(result) << endl;
	
	cout << "Sum=" << sumMethod->combine(result)
		<< " Log=" << logMethod->combine(result) << endl;
	
	/* save gripper */
	Gripper::Ptr grp = builder->parametersToGripper(param);
	GripperXMLLoader::save(grp, "obj.grp.xml");
	
	return 0;
}
