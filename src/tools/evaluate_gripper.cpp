#include <iostream>
#include <rw/rw.hpp>
#include <rw/RobWork.hpp>
#include <rw/common/Log.hpp>
#include <rw/math/Math.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include <context/TaskDescription.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include <rwsim/loaders/DynamicWorkCellLoader.hpp>
#include <models/Gripper.hpp>
#include <models/loaders/MasterGripperLoader.hpp>
#include <process/GripperEvaluationManagerFactory.hpp>
#include <grasps/SurfaceSample.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace rw;
using namespace rw::common;
using namespace rw::math;
using namespace rwsim::dynamics;
using namespace rwsim::loaders;
using namespace boost::program_options;
using namespace gripperz::loaders;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace gripperz::context;
using namespace gripperz::process;
using namespace gripperz::grasps;

/******************************************************************************/
struct Configuration {
    int threads;
    int ngrasps;
    string dwc_filename;
    string td_filename;
    string gripper_filename;
    string result_filename;
    string index_calculators;
} CONFIG;

/******************************************************************************/
struct Data {
    DynamicWorkCell::Ptr dwc;
    TaskDescription::Ptr td;
    Gripper::Ptr gripper;
} DATA;

/******************************************************************************/
void initialize() {
    Math::seed();
    RobWork::getInstance()->initialize();
    Log::log().setLevel(Log::Info);
}

/******************************************************************************/
bool parse_cli(int argc, char* argv[], Configuration& conf) {
    string values;
    string offset;

    options_description desc("Options:");
    desc.add_options()
            ("help,h", "help message")
            ("threads,t", value<int>(&conf.threads)->default_value(1), "number of threads to use")
            ("ngrasps,n", value<int>(&conf.ngrasps)->default_value(100), "number of grasps to perform")
            ("dwc", value<string>(&conf.dwc_filename)->required(), "dynamic workcell file")
            ("td", value<string>(&conf.td_filename)->required(), "task description file")
            ("gripper,g", value<string>(&conf.gripper_filename)->required(), "base gripper file")
            ("result,r", value<string>(&conf.result_filename), "result gripper file")
            ("indices,i", value<string>(&conf.index_calculators)->default_value("success"), "quality indices to compute");
    variables_map vm;

    string usage = "Usage: ";

    try {
        store(command_line_parser(argc, argv).options(desc).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << usage << endl;
            cout << desc << endl;
            return false;
        }

    } catch (exception& e) {
        cout << usage << endl;
        cout << desc << endl;
        return false;
    }
    
    if (conf.result_filename.empty()) {
        conf.result_filename = conf.gripper_filename;
    }

    return true;
}

/******************************************************************************/
void load_data(const Configuration& config, Data& data) {
    INFO << "* Loading dwc... ";
    data.dwc = DynamicWorkCellLoader::load(config.dwc_filename);
    INFO << "Loaded." << endl;

    INFO << "* Loading task description... ";
    data.td = TaskDescriptionLoader::load(config.td_filename, data.dwc);
    INFO << "Loaded." << endl;

    INFO << "* Loading gripper... ";
    GripperLoader::Ptr loader = new MasterGripperLoader();
    data.gripper = loader->load(config.gripper_filename);
    INFO << "Loaded." << endl;
    INFO << "Gripper name: " << data.gripper->getName() << endl;
}

/******************************************************************************/
GripperEvaluationProcessManager::Ptr make_evaluation_manager(const Configuration& config, const Data& data) {
    GripperEvaluationProcessManager::Ptr manager = GripperEvaluationManagerFactory::makeStandardEvaluationManager(
            data.td,
            config.ngrasps,
            config.threads,
            vector<SurfaceSample>()
            );
    
    return manager;
}

/******************************************************************************/
GripperQuality::Ptr evaluate_gripper(Gripper::Ptr gripper, GripperEvaluationProcessManager::Ptr manager) {
    GripperQuality::Ptr quality = manager->evaluateGripper(gripper);
    gripper->setQuality(quality);
    
    return quality;
}

/******************************************************************************/
int main(int argc, char* argv[]) {
    initialize();

    if (!parse_cli(argc, argv, CONFIG)) {
        return -1;
    }

    try {
        load_data(CONFIG, DATA);
    } catch (exception& e) {
        INFO << "Exception during loading data: " << e.what() << endl;
        return -1;
    }
    
    Gripper::Ptr gripper = DATA.gripper;
    GripperEvaluationProcessManager::Ptr manager = make_evaluation_manager(CONFIG, DATA);
    GripperQuality::Ptr quality = evaluate_gripper(gripper, manager);
    
    INFO << *quality << endl;
    
    GripperLoader::Ptr loader = new MasterGripperLoader();
    loader->save(CONFIG.result_filename, gripper);

    return 0;
}