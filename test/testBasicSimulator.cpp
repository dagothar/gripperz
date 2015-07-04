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
#include <context/TaskDescription.hpp>
#include <loaders/TaskDescriptionLoader.hpp>


using namespace std;
using namespace gripperz::simulator;
using namespace gripperz::context;
using namespace gripperz::loaders;
USE_ROBWORK_NAMESPACE
using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
using namespace robworksim;
using namespace rwlibs::task;


int main(int argc, char* argv[]) {
	/* load workcell & context */
	cout << "Loading workcell..." << endl;
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load("../data/rotor/Scene.dwc.xml");
	cout << "Loading context..." << endl;
	TaskDescription::Ptr td = TaskDescriptionLoader::load("../data/rotor/task1.td.xml", dwc);
	
	/* load tasks */
	cout << "Loading tasks..." << endl;
	GraspTask::Ptr tasks = GraspTask::load("../data/tasks/rotor1_67.tasks.xml");
	
	/* create simulator */
	GripperSimulator::Ptr simulator = new InterferenceSimulator(dwc, td->getInterferenceLimit(), td->getInterferenceObjects());
	
	/* run simulation */
	cout << "Running simulation..." << endl;
	simulator->loadTasks(tasks);
	simulator->start(dwc->getWorkcell()->getDefaultState());
	
	while (simulator->isRunning()) {}
	
	cout << "Done." << endl;
	
	return 0;
}
