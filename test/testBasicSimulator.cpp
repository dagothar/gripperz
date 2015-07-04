/**
 * @file testBasicSimulator.hpp
 * @author Adam Wolniakowski
 * @date 01-07-2015
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <simulator/BasicSimulator.hpp>
#include <simulator/InterferenceSimulator.hpp>


using namespace std;
using namespace gripperz::simulator;
USE_ROBWORK_NAMESPACE
using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
using namespace robworksim;
using namespace rwlibs::task;


int main(int argc, char* argv[]) {
	/* load workcell & context */
	cout << "Loading workcell..." << endl;
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load("../data/rotor/Scene.dwc.xml");
	
	/* load tasks */
	cout << "Loading tasks..." << endl;
	GraspTask::Ptr tasks = GraspTask::load("../data/tasks/rotor1_67.tasks.xml");
	
	/* create simulator */
	GripperSimulator::Ptr simulator = new InterferenceSimulator(dwc, 1);
	
	/* run simulation */
	cout << "Running simulation..." << endl;
	simulator->loadTasks(tasks);
	simulator->start(dwc->getWorkcell()->getDefaultState());
	
	while (simulator->isRunning()) {}
	
	cout << "Done." << endl;
	
	return 0;
}
