#include <iostream>
#include <gripperz.hpp>

using namespace std;
USE_ROBWORK_NAMESPACE
        using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
        using namespace robworksim;
USE_GRIPPERZ_NAMESPACE
        using namespace gripperz;

struct Configuration {
    string dwcFilename;
    string graspDatabaseFilename;
} CONFIG;

int main(int argc, char* argv[]) {
    /* INITIALIZE ROBWORK */
    Math::seed();
    RobWork::getInstance()->initialize();
    Log::log().setLevel(Log::Info);

    /* CONFIGURATION */
    CONFIG.dwcFilename = "/home/dagothar/gripperz-dev/data/rotor/Scene.dwc.xml";
    CONFIG.graspDatabaseFilename = "tasks.xml";

    /* LOAD DATA */
    DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load(CONFIG.dwcFilename);

    /* GRASP SOURCE */
    GraspSource::Ptr grasp_source = new RWGraspDatabase(CONFIG.graspDatabaseFilename);

    /* GRASP SIMULATOR */
    GripperSimulator::Ptr gripper_simulator = new BasicSimulator(dwc, 1);
    
    /* GRASP EVALUATOR */
    GripperEvaluator::Ptr gripper_evaluator = new GripperEvaluator();
    gripper_evaluator->addQualityIndexCalculator("success", new SuccessIndexCalculator());

    return 0;
}


