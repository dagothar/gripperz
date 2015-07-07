/**
 * @file paramLandscape.cpp
 * @author Adam Wolniakowski
 * @date 6-7-2015
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <sstream>
#include <fstream>
#include <evaluation/GripperObjectiveFunction.hpp>
#include <evaluation/GripperEvaluationManager.hpp>
#include <evaluation/GripperEvaluationManagerFactory.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <simulation/InterferenceSimulator.hpp>
#include <optimization/CombineObjectivesFactory.hpp>
#include <models/MapGripperBuilder.hpp>


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::optimization;
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
		sstr << v[i] << ", ";
	}
	
	return sstr.str();
}


int main(int argc, char* argv[]) {
	Math::seed();
	RobWork::getInstance()->initialize();
	//Log::log().setLevel(Log::Debug);
	
	/* load data */
	cout << "Loading workcell..." << endl;
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load("../data/rotor/Scene.dwc.xml");
	cout << "Loading context..." << endl;
	TaskDescription::Ptr td = TaskDescriptionLoader::load("../data/rotor/task3.td.xml", dwc);
	cout << "Loading gripper..." << endl;
	Gripper::Ptr gripper = GripperXMLLoader::load("../data/grippers/template.grp.xml");
	
	/* read weights */
	cout << "Please input 7 weights (success, robustness, alignment, coverage, wrench, stress, volume): ";
	vector<double> weights = readVector(cin);
	cout << "Weights: " << vectorToString(weights) << endl;
	
	/* create objective function */
	vector<MapGripperBuilder::ParameterName> params{
		MapGripperBuilder::CutTilt
	};
	GripperBuilder::Ptr builder = new MapGripperBuilder(gripper, params);
	
	GripperEvaluationManager::Ptr manager = GripperEvaluationManagerFactory::getEvaluationManager(td, 100);
	MultiObjectiveFunction::Ptr func = new GripperObjectiveFunction(builder, manager);
	
	/* initialize combiners */
	CombineObjectives::Ptr sumMethod = CombineObjectivesFactory::make("sum", weights);
	CombineObjectives::Ptr logMethod = CombineObjectivesFactory::make("log", weights);
	
	/* simulate grippers */
	ofstream results("results.txt");
	unsigned n = 0;
	for (double x = -90.0; x < 90.0; x += 10) {	
		n++;
		
		/* evaluate */
		vector<double> param{x};
		vector<double> result = (*func)(param);
		
		cout << "Objectives (success, robustness, alignment, coverage, wrench, stress, volume): " << vectorToString(result) << endl;
		
		double q_sum = sumMethod->combine(result);
		double q_log = logMethod->combine(result);
		cout << "Sum=" << q_sum << " Log=" << q_log << endl;
		
		/* save gripper */
		stringstream sstr;
		sstr << "step" << n << ".grp.xml";
		Gripper::Ptr grp = builder->parametersToGripper(param);
		GripperXMLLoader::save(grp, sstr.str());
		
		/* save results */
		results << n << ", " << x << ", " << vectorToString(result) << " " << q_sum << ", " << q_log << endl;
		
	}
	
	results.close();
	
	return 0;
}
