#include <iostream>
#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <context/Context.hpp>
#include <grasps/databases/RWGraspDatabase.hpp>
#include <simulation/BasicSimulator.hpp>
#include <evaluation/GripperEvaluator.hpp>
#include <evaluation/calculators.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <process/StandardGripperEvaluationProcessManager.hpp>

using namespace std;
USE_ROBWORK_NAMESPACE
        using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
        using namespace robworksim;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;
using namespace gripperz::process;
using namespace gripperz::grasps;
using namespace gripperz::models;
using namespace gripperz::loaders;
using namespace gripperz::grasps::databases;

struct Configuration {
    string dwcFilename;
    string graspDatabaseFilename;
    string gripperFilename;
    double alignmentFilteringRadius;
    double stressLimit;
    double volumeLimit;
} CONFIG;

int main(int argc, char* argv[]) {
    /* INITIALIZE ROBWORK */
    Math::seed();
    RobWork::getInstance()->initialize();
    Log::log().setLevel(Log::Debug);

    /* CONFIGURATION */
    CONFIG.dwcFilename = "/home/dagothar/gripperz-dev/data/rotor/Scene.dwc.xml";
    CONFIG.graspDatabaseFilename = "/home/dagothar/gripperz-dev/data/tasks/tasks.xml";
    CONFIG.gripperFilename = "/home/dagothar/gripperz-dev/data/grippers/gripper.grp.xml";
    CONFIG.alignmentFilteringRadius = 0.01;
    CONFIG.stressLimit = 10; // 10 MPa
    CONFIG.volumeLimit = 200; // cm3

    /* CONTEXT */
    DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load(CONFIG.dwcFilename);
    Context::Ptr context = new Context(dwc);

    /* GRASP SOURCE */
    GraspSource::Ptr grasp_source = new RWGraspDatabase(CONFIG.graspDatabaseFilename);

    /* GRASP SIMULATOR */
    GripperSimulator::Ptr gripper_simulator = new BasicSimulator(context->getDynamicWorkCell(), 1);

    /* GRASP EVALUATOR */
    GripperEvaluator::Ptr gripper_evaluator = new GripperEvaluator();
    gripper_evaluator->addQualityIndexCalculator("success", new SuccessIndexCalculator());
    gripper_evaluator->addQualityIndexCalculator("alignment", new AlignmentIndexCalculator(CONFIG.alignmentFilteringRadius));
    gripper_evaluator->addQualityIndexCalculator("coverage", new CoverageIndexCalculator());
    gripper_evaluator->addQualityIndexCalculator("wrench", new WrenchIndexCalculator());
    gripper_evaluator->addQualityIndexCalculator("stress", new StressIndexCalculator(CONFIG.stressLimit));
    gripper_evaluator->addQualityIndexCalculator("volume", new VolumeIndexCalculator(CONFIG.volumeLimit));

    /* EVALUATION MANAGER */
    GripperEvaluationProcessManager::Ptr evaluation_manager =
            new StandardGripperEvaluationProcessManager(
                                                        context,
                                                        grasp_source,
                                                        gripper_simulator,
                                                        gripper_evaluator
                                                        );

    /* EVALUATE GRIPPER */
    Gripper::Ptr gripper = GripperXMLLoader::load(CONFIG.gripperFilename);
    GripperQuality::Ptr quality = evaluation_manager->evaluateGripper(gripper);

    cout << *quality << endl;

    return 0;
}


