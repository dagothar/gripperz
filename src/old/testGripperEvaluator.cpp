/**
 * @file testGripperEvaluator.hpp
 * @author Adam Wolniakowski
 * @date 06-07-2015
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <evaluation/OldGripperEvaluator.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <grasps/GraspSource.hpp>
#include <simulation/InterferenceSimulator.hpp>
#include <grasps/planners/BasicParallelGripperGraspPlanner.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::simulation;
using namespace gripperz::context;
using namespace gripperz::loaders;
using namespace gripperz::models;
using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace rwlibs::task;
USE_ROBWORK_NAMESPACE
using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
using namespace robworksim;



int main(int argc, char* argv[]) {
	/* initialize RW */
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Info);
	
	/* load data */
	cout << "Loading workcell..." << endl;
	DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load("../data/rotor/Scene.dwc.xml");
	cout << "Loading context..." << endl;
	TaskDescription::Ptr td = TaskDescriptionLoader::load("../data/rotor/task1.td.xml", dwc);
	cout << "Loading gripper..." << endl;
	OldGripper::Ptr gripper = GripperXMLLoader::load("../data/grippers/template.grp.xml");
	
	gripper->updateGripper(
		td->getWorkCell(),
		td->getDynamicWorkCell(),
		td->getGripperDevice(),
		td->getGripperDynamicDevice(),
		td->getInitState(),
		td->getGripperTCP()
	);
	
	/* create evaluation objects */
	GripperSimulator::Ptr simulator = new InterferenceSimulator(dwc, td->getInterferenceLimit(), td->getInterferenceObjects());
	OldGripperEvaluator::Ptr evaluator = new OldGripperEvaluator(td, td->getAlignmentCalculator());
	
	/* generate tasks */
	GraspSource::Ptr generator = new BasicParallelGripperGraspPlanner(100, td->getInitState(), td);
	GraspTask::Ptr tasks = generator->getGrasps();
	
	/* simulate tasks */
	simulator->loadTasks(tasks);
	simulator->start(td->getInitState());
	
	while (simulator->isRunning()) {}
	
	/* evaluate gripper */
	Log::log().setLevel(Log::Debug);
	GripperQuality::Ptr quality = evaluator->evaluate(gripper, simulator->getTasks());
	gripper->setQuality(quality);
	
	INFO << "\nRESULTS" << endl;
	//INFO << gripper->getQuality() << endl;
	
	return 0;
}
