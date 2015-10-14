/**
 * @file gripper_evaluate.cpp
 * @author Adam Wolniakowski
 * @date 29-07-2015
 */

#include <iostream>
#include <vector>
#include <string>
#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <gripperz.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
USE_ROBWORK_NAMESPACE
        using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
        using namespace robworksim;
USE_GRIPPERZ_NAMESPACE
        using namespace gripperz;
using namespace boost::program_options;
namespace po = boost::program_options;

struct Configuration {
    int cores;
    int grasps;
    int robustness;
    vector<int> parameters;
    vector<double> values;
    vector<double> weights;
    string method;
    string dwc;
    string td;
    string gripper;
    string samples;
    string name;
    string task_file;
    double sigma_a;
    double sigma_p;
    Transform3D<> offset;

    Configuration() :
    //parameters({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}),
    weights({1, 1, 1, 1, 1, 1, 1}) {
    }
} configuration;

struct Data {
    DynamicWorkCell::Ptr dwc;
    TaskDescription::Ptr td;
    vector<SurfaceSample> samples;
    OldGripper::Ptr gripper;
} data;

void initialize() {
    Math::seed();
    RobWork::getInstance()->initialize();
    Log::log().setLevel(Log::Info);
}

bool parse_cli(int argc, char* argv[], Configuration& conf) {
    string values;
    string offset;

    string usage =
            "This script evaluates gripper constructed from parameter values.\n\n"
            "Usage:\n"
            "\tgripper_evaluate\n\n"
            "Example:\n"
            "\t./gripper_evaluate -c 4 -g 100 -r 0 -v 0.1 0.025 0.02 1 45 0.01 120 0 0.025 0 0.05 25 --dwc ~/grippers/scenes/rotor/Scene.dwc.xml "
            "--td ~/grippers/scenes/rotor/task1.td.xml --samples ~/grippers/scenes/rotor/samples1.xml\n";
    options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "help message")
            ("cores,c", value<int>(&conf.cores)->default_value(1), "number of threads to use")
            ("grasps,g", value<int>(&conf.grasps)->default_value(100), "number of grasps to perform")
            ("robustness,r", value<int>(&conf.robustness)->default_value(100), "number of robustness tasks to generate")
            ("parameters,p", value<vector<int> >(&conf.parameters)->multitoken(), "gripper parameter indices")
            ("values,v", value<string>(&values), "gripper parameter values in a quoted string")
            ("weights,w", value<vector<double> >(&conf.weights)->multitoken(), "7 weights for objectives (0-6)")
            ("method,m", value<string>(&conf.method)->default_value("product"), "method for combining objectives (sum, product, log)")
            ("dwc", value<string>(&conf.dwc)->required(), "dynamic workcell file")
            ("td", value<string>(&conf.td)->required(), "task description file")
            ("samples", value<string>(&conf.samples), "surface samples file")
            ("gripper", value<string>(&conf.gripper), "base gripper file")
            ("save_tasks", value<string>(&conf.task_file), "save tasks to file (optional)")
            ("name", value<string>(&conf.name)->default_value("gripper"), "gripper name")
            ("sigma_a", value<double>(&conf.sigma_a)->default_value(8), "standard deviation in of angle in degrees")
            ("sigma_p", value<double>(&conf.sigma_p)->default_value(0.003), "standard deviation of position in meters")
            ("offset", value<string>(&offset), "offset noise for the grasps (6 values: x, y, z, r, p, y)");
    variables_map vm;

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

    istringstream sstr(values);
    double v;
    while (sstr >> v) {
        conf.values.push_back(v);
    }

    if (conf.parameters.size() != conf.values.size()) {
        RW_THROW("Parameters and values size mismatch!");
    }

    if (!offset.empty()) {
        istringstream offset_sstr(offset);
        double x, y, z, roll, pitch, yaw;

        offset_sstr >> x >> y >> z >> roll >> pitch >> yaw;

        conf.offset = Transform3D<>(Vector3D<>(x, y, z), RPY<>(roll * Deg2Rad, pitch * Deg2Rad, yaw * Deg2Rad));
    }

    return true;
}

GripperObjectiveFunction::Ptr make_objective_function(const Configuration& config, const Data& data) {
    /* evaluation manager */
    GripperEvaluationProcessManager::Configuration configuration;
    configuration.nOfGraspsPerEvaluation = config.grasps;
    configuration.nOfRobustnessTargets = config.robustness;
    configuration.sigma_a = config.sigma_a;
    configuration.sigma_p = config.sigma_p;
    StandardGripperEvaluationProcessManager::Ptr manager = GripperEvaluationManagerFactory::makeStandardEvaluationManager(data.td, configuration, config.cores, data.samples);

    GraspFilterChain::Ptr filterChain = new GraspFilterChain();
    GraspFilter::Ptr offsetFilter = new GraspOffsetFilter(config.offset);
    filterChain->addFilter(offsetFilter);

    GraspSource::Ptr graspSource = manager->getGraspSource();
    graspSource = new FilteredGraspSource(graspSource, filterChain);
    manager->setGraspSource(graspSource);

    /* gripper builder */
    vector<MapGripperBuilder::ParameterName> parameters;
    vector<string> names;

    BOOST_FOREACH(int id, config.parameters) {
        MapGripperBuilder::ParameterName p = static_cast<MapGripperBuilder::ParameterName> (id);
        parameters.push_back(p);
        names.push_back(MapGripperBuilder::parameterNameToString(p));
    }
    GripperBuilder::Ptr builder = new MapGripperBuilder(data.gripper, parameters);

    /* objective */
    GripperObjectiveFunction::Ptr objective = new GripperObjectiveFunction(builder, manager);

    return objective;
}

void load_data(const Configuration& config, Data& data) {
    INFO << "* Loading dwc... ";
    data.dwc = DynamicWorkCellLoader::load(config.dwc);
    INFO << "Loaded." << endl;
    INFO << "* Loading task description... ";
    data.td = TaskDescriptionLoader::load(config.td, data.dwc);
    INFO << "Loaded." << endl;

    if (!config.samples.empty()) {
        INFO << "* Loading samples... ";
        data.samples = SurfaceSample::loadFromXML(config.samples);
        INFO << "Loaded." << endl;
    }

    if (!config.gripper.empty()) {
        INFO << "* Loading gripper... ";
        data.gripper = GripperXMLLoader::load(config.gripper);
        INFO << "Loaded." << endl;
    } else {
        data.gripper = ownedPtr(new OldGripper());
    }
}

int main(int argc, char* argv[]) {
    initialize();

    if (!parse_cli(argc, argv, configuration)) {
        return -1;
    }

    try {
        load_data(configuration, data);
    } catch (exception& e) {
        INFO << "Exception during loading data: " << e.what() << endl;
        return -1;
    }

    GripperObjectiveFunction::Ptr objective = make_objective_function(configuration, data);

    CombineObjectives::Ptr method = CombineObjectivesFactory::make(configuration.method, configuration.weights);

    INFO << "* Evaluating gripper:" << endl;
    INFO << *objective->getBuilder()->parametersToGripper(configuration.values);

    vector<double> results = objective->evaluate(configuration.values);

    OldGripper::Ptr gripper = objective->getLastGripper();
    GripperQuality::Ptr quality = gripper->getQuality();
    quality->setIndex("quality", method->combine(results));
    gripper->setQuality(quality);

    INFO << "* Results:" << endl;
    INFO << *quality << endl;

    gripper->setName(configuration.name);
    GripperXMLLoader::save(gripper, configuration.name + ".grp.xml");

    /* save tasks if specified */
    if (configuration.task_file != "") {
        StandardGripperEvaluationProcessManager::Ptr manager = dynamic_cast<StandardGripperEvaluationProcessManager*> (objective->getEvaluationManager().get());
        rwlibs::task::GraspTask::Ptr tasks = manager->getSimulator()->getTasks();

        rwlibs::task::GraspTask::saveRWTask(tasks, configuration.task_file);
    }

    return 0;
}
