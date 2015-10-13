/**
 * @file generate_grippers.cpp
 * @author Adam Wolniakowski
 * @date 11-07-2015
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>
#include <rw/math/Math.hpp>
#include <rw/rw.hpp>
#include <rw/RobWork.hpp>
#include <rw/common/Log.hpp>
#include <rw/common/ExtensionRegistry.hpp>
#include <rwsim/rwsim.hpp>
#include <rwsim/loaders/DynamicWorkCellLoader.hpp>
#include <rw/loaders/model3d/STLFile.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>
#include <rwsimlibs/ode/ODEPlugin.hpp>

#include <context/TaskDescription.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include <models/OldGripper.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <grasps/GraspSource.hpp>
#include <simulation/InterferenceSimulator.hpp>
#include <evaluation/OldGripperEvaluator.hpp>
#include <evaluation/ConstrainedGripperEvaluator.hpp>
#include <models/MapGripperBuilder.hpp>
#include <evaluation/GripperObjectiveFunction.hpp>
#include <process/GripperEvaluationManager.hpp>
#include <process/GripperEvaluationManagerFactory.hpp>
#include <math/CombineObjectivesFactory.hpp>
#include <math/CombinedFunction.hpp>


#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
USE_ROBWORK_NAMESPACE
        using namespace robwork;
using namespace rw::models;
using namespace rw::common;
using namespace rw::loaders;
using namespace rwlibs::task;
using namespace rwsim;
using namespace rwsim::dynamics;
using namespace rwsim::loaders;
using namespace boost::program_options;
namespace po = boost::program_options;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::simulation;
using namespace gripperz::math;
using namespace gripperz::evaluation;
using namespace gripperz::process;
using namespace gripperz::grasps;
using namespace gripperz::loaders;

string vectorToString(const vector<double>& v) {
    stringstream sstr;

    for (unsigned i = 0; i < v.size(); ++i) {
        sstr << v[i] << ", ";
    }

    return sstr.str();
}

int main(int argc, char* argv[]) {
    /* initialize robwork */
    Math::seed();
    RobWork::getInstance()->initialize();
    Log::log().setLevel(Log::Info);

    /* parameter bounds */
    map<MapGripperBuilder::ParameterName, pair<double, double> > bounds;
    bounds[MapGripperBuilder::Length] = make_pair(0.0, 0.2);
    bounds[MapGripperBuilder::Width] = make_pair(0.0, 0.05);
    bounds[MapGripperBuilder::Depth] = make_pair(0.0, 0.05);
    bounds[MapGripperBuilder::ChfDepth] = make_pair(0.0, 1.0);
    bounds[MapGripperBuilder::ChfAngle] = make_pair(0.0, 90.0);
    bounds[MapGripperBuilder::CutDepth] = make_pair(0.0, 0.05);
    bounds[MapGripperBuilder::CutAngle] = make_pair(0.0, 180.0);
    bounds[MapGripperBuilder::CutTilt] = make_pair(-90.0, 90.0);
    bounds[MapGripperBuilder::TcpOffset] = make_pair(0.0, 0.2);
    bounds[MapGripperBuilder::Jawdist] = make_pair(0.0, 0.05);
    bounds[MapGripperBuilder::Stroke] = make_pair(0.0, 0.05);
    bounds[MapGripperBuilder::Force] = make_pair(0.0, 20.0);

    /* options */
    int cores, ntargets, nrobust;
    string dwcFilename;
    string tdFilename;
    int ngrippers;
    int last = 0;
    string name;
    string samplesFilename;
    string gripper_filename;
    vector<double> weights{1, 1, 1, 1, 1, 1, 1};
    vector<int> parameters{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    /* define CLI options */
    string usage =
            "This is a script used to generate tasks for a single gripper, simulate them and"
            " evaluate gripper's performance.\n\n"
            "Usage:\n"
            "evaluate-gripper";
    options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "help message")
            ("cores,c", value<int>(&cores)->default_value(1), "number of threads to use")
            ("ntargets,t", value<int>(&ntargets)->default_value(100), "number of tasks to simulate")
            ("nrobust,r", value<int>(&nrobust)->default_value(100), "number of robustness tasks to generate")
            ("weights,w", value<vector<double> >(&weights)->multitoken(), "7 weights for objectives (0-6)")
            ("parameters,p", value<vector<int> >(&parameters)->multitoken(), "12 parameters to optimize (0-11)")
            ("gripper,g", value<string>(&gripper_filename)->required(), "gripper file")
            ("dwc", value<string>(&dwcFilename)->required(), "dynamic workcell file")
            ("td", value<string>(&tdFilename)->required(), "task description file")
            ("name", value<string>(&name)->required(), "name for grippers")
            ("ngrippers,n", value<int>(&ngrippers)->required(), "number of grippers to generate")
            ("last,l", value<int>(&last)->default_value(0), "last generated index (to continue after crash)")
            ("ssamples,s", value<string>(&samplesFilename), "surface samples file");
    variables_map vm;

    /* parse CLI */
    try {
        store(command_line_parser(argc, argv).options(desc).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << usage << endl;
            cout << desc << endl;
            return 0;
        }

    } catch (...) {
        cout << usage << endl;
        cout << desc << endl;
        return -1;
    }

    /* load data */
    INFO << "* Loading dwc... ";
    DynamicWorkCell::Ptr dwc = DynamicWorkCellLoader::load(dwcFilename);
    INFO << "Loaded." << endl;
    INFO << "* Loading task description... " << tdFilename << " " << endl;
    TaskDescription::Ptr td = TaskDescriptionLoader::load(tdFilename, dwc);
    INFO << "Loaded." << endl;
    INFO << "* Loading gripper... " << gripper_filename;
    OldGripper::Ptr gripper = GripperXMLLoader::load(gripper_filename);
    INFO << " Loaded." << endl;

    /* parameters */
    vector<MapGripperBuilder::ParameterName> params;

    BOOST_FOREACH(int id, parameters) {
        params.push_back(static_cast<MapGripperBuilder::ParameterName> (id));
    }

    Q lower(parameters.size()), upper(parameters.size());
    for (unsigned i = 0; i < parameters.size(); ++i) {
        lower(i) = bounds[params[i]].first;
        upper(i) = bounds[params[i]].second;
    }

    vector<SurfaceSample> ssamples;
    if (vm.count("ssamples")) {
        INFO << "* Loading samples... ";
        ssamples = SurfaceSample::loadFromXML(samplesFilename);
        INFO << "Loaded." << endl;
    }

    /* construct objective function */
    GripperEvaluationManager::Configuration config;
    config.nOfGraspsPerEvaluation = ntargets;
    config.nOfRobustnessTargets = nrobust;
    StandardEvaluationManager::Ptr manager = GripperEvaluationManagerFactory::makeStandardEvaluationManager(td, config, cores, ssamples);
    OldGripperEvaluator::Ptr evaluator = new ConstrainedGripperEvaluator(td, td->getAlignmentCalculator());
    manager->setEvaluator(evaluator);

    GripperBuilder::Ptr builder = new MapGripperBuilder(gripper, params);
    GripperObjectiveFunction::Ptr func = new GripperObjectiveFunction(builder, manager);

    CombineObjectives::Ptr logMethod = CombineObjectivesFactory::make("product", weights);
    ObjectiveFunction::Ptr objective = new CombinedFunction(func, logMethod);

    /* generate grippers */
    unsigned tries = last;
    int generated = 0;
    while (generated < ngrippers) {
        INFO << "# Trying gripper " << ++tries << endl;
        INFO << "# Grippers generated so far: " << generated << endl;

        /* randomize parameters */
        Q qparams = Math::ranQ(lower, upper);
        vector<double> vparams = Math::toStdVector(qparams, params.size());

        /* test gripper */
        double q = 0;
        try {
            q = objective->evaluate(vparams);
            INFO << "Gripper quality= " << q << endl;
        } catch (exception& e) {
            RW_WARN("Exception during gripper evaluation: " << e.what());
        }

        stringstream sstr;
        sstr << name << "_" << tries;
        OldGripper::Ptr grp = func->getLastGripper();
        grp->setName(sstr.str());
        grp->getQuality().quality = q;

        if (q > 0.0) {
            ++generated;

            string fname = sstr.str() + ".grp.xml";
            GripperXMLLoader::save(grp, fname);

            INFO << "Gripper OK: " << fname << endl;
        } else {
            string fname = "_" + sstr.str() + ".grp.xml";
            GripperXMLLoader::save(grp, fname);

            INFO << "Gripper NOT OK: " << fname << endl;
        }
    }

    INFO << "Finished. Generated " << generated << " grippers with " << tries << " tries." << endl;

    return 0;
}
