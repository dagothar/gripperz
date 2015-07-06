/**
 * @file testGripperObjective.hpp
 * @author Adam Wolniakowski
 * @date 01-07-2015
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <evaluation/GripperObjectiveFunction.hpp>
#include <loaders/TaskDescriptionLoader.hpp>


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::optimization;
using namespace gripperz::context;
using namespace gripperz::loaders;
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
	/* load data */
	cout << "Loading workcell..." << endl;
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load("../data/rotor/Scene.dwc.xml");
	cout << "Loading context..." << endl;
	TaskDescription::Ptr td = TaskDescriptionLoader::load("../data/rotor/task1.td.xml", dwc);
	
	/* create objective function */
	MultiObjectiveFunction::Ptr func = new GripperObjectiveFunction(td);
	
	/* read grippers */
	unsigned n = 0;
	while (true) {
		cout << "#" << n << "> ";
		cout << "Please input 9 parameters (length, width, depth, chf. depth, chf. angle, cut depth, cut angle, tilt, tcp): ";
		vector<double> param = readVector(cin);
		
		/* evaluate */
		vector<double> result = (*func)(param);
		
		cout << "Objectives (success, robustness, alignment, coverage, wrench, stress, volume): " << vectorToString(result) << endl;
	}
	
	return 0;
}