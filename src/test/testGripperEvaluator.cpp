/**
 * @file testGripperEvaluator.hpp
 * @author Adam Wolniakowski
 * @date 06-07-2015
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <evaluation/GripperEvaluator.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <grasps/TaskGenerator.hpp>
#include <simulation/InterferenceSimulator.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::simulation;
using namespace gripperz::context;
using namespace gripperz::loaders;
using namespace gripperz::models;
using namespace gripperz::grasps;
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
	Gripper::Ptr gripper = GripperXMLLoader::load("../data/grippers/template.grp.xml");
	
	gripper->updateGripper(
		td->getWorkCell(),
		td->getDynamicWorkCell(),
		td->getGripperDevice(),
		td->getGripperDynamicDevice(),
		td->getInitState(),
		td
	);
	
	/* create evaluation objects */
	GripperSimulator::Ptr simulator = new InterferenceSimulator(dwc, td->getInterferenceLimit(), td->getInterferenceObjects());
	GripperEvaluator::Ptr evaluator = new GripperEvaluator(td);
	
	/* generate tasks */
	TaskGenerator::Ptr generator = new TaskGenerator(td);
	generator->generateTask(100, td->getInitState());
	GraspTask::Ptr tasks = generator->getTasks();
	GraspTask::Ptr samples = generator->getSamples();
	
	/* simulate tasks */
	simulator->loadTasks(tasks);
	simulator->start(td->getInitState());
	
	while (simulator->isRunning()) {}
	
	/* evaluate gripper */
	Log::log().setLevel(Log::Debug);
	GripperQuality::Ptr quality = evaluator->evaluateGripper(gripper, simulator->getTasks(), samples);
	gripper->setQuality(*quality);
	
	INFO << "\nRESULTS" << endl;
	INFO << gripper->getQuality() << endl;
	
	return 0;
}
