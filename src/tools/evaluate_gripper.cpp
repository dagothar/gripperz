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
#include <models/ParametrizedGripper.hpp>
#include <models/loaders/MasterGripperLoader.hpp>
#include <process/GripperEvaluationManagerFactory.hpp>
#include <evaluation/GripperEvaluator.hpp>
#include <evaluation/IndexGripperQualityExtractor.hpp>
#include <evaluation/calculators.hpp>
#include <grasps/SurfaceSample.hpp>
#include <boost/tokenizer.hpp>
#include <math/CombineObjectivesFactory.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <grasps/Types.hpp>
#include <grasps/databases/RWGraspDatabase.hpp>
#include <grasps/filters.hpp>
#include <grasps/decorators.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace rw;
using namespace rw::common;
using namespace rw::math;
using namespace rwsim::dynamics;
using namespace rwsim::loaders;
using namespace rwlibs::task;
using namespace boost::program_options;
using namespace gripperz::loaders;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace gripperz::context;
using namespace gripperz::process;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace gripperz::grasps::databases;
using namespace gripperz::grasps::decorators;
using namespace gripperz::math;
using namespace gripperz::parametrization;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

/******************************************************************************/
struct Configuration {
    int threads;
    int ngrasps;
    string dwc_filename;
    string td_filename;
    string gripper_filename;
    string result_filename;
    vector<string> index_calculators;
    vector<double> weights;
    string method;
    vector<string> parameters;
    vector<double> values;
    string grasps_filename;
    string saved_grasps_filename;
    string ssamples;

    double covPosFilteringRadius;
    double covAngleFilteringRadius;
    double aliFilteringRadius;
    double stressLimit;
    double volumeLimit;
    double sigma_p;
    double sigma_a;
    unsigned seed;
} CONFIG;

/******************************************************************************/
struct Data {
    DynamicWorkCell::Ptr dwc;
    TaskDescription::Ptr td;
    Gripper::Ptr gripper;
    vector<SurfaceSample> ssamples;
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
    string indices;
    string parameters;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "help message")
            ("threads,t", value<int>(&conf.threads)->default_value(1), "number of threads to use")
            ("ngrasps,n", value<int>(&conf.ngrasps)->default_value(100), "number of grasps to perform")
            ("dwc", value<string>(&conf.dwc_filename)->required(), "dynamic workcell file")
            ("td", value<string>(&conf.td_filename)->required(), "task description file")
            ("gripper,g", value<string>(&conf.gripper_filename)->required(), "base gripper file")
            ("result,r", value<string>(&conf.result_filename), "result gripper file")
            ("indices,i", value<string>(&indices)->default_value("success,alignment,coverage,wrench,stress,volume"), "quality indices to compute")
            ("weights,w", value<vector<double> >(&conf.weights)->multitoken()->default_value(vector<double>{1, 1, 1, 1, 1, 1}, "1 1 1 1 1 1"), "weights for individual objectives")
            ("method,m", value<string>(&conf.method)->default_value("product"), "method for combining objectives (sum, product, log)")
            ("covP", value<double>(&conf.covPosFilteringRadius)->default_value(0.001), "pos. filtering radius for coverage")
            ("covA", value<double>(&conf.covAngleFilteringRadius)->default_value(15), "angle filtering radius for coverage")
            ("aliR", value<double>(&conf.aliFilteringRadius)->default_value(0.01), "filtering radius for alignment")
            ("maxS", value<double>(&conf.stressLimit)->default_value(10), "stress limit")
            ("maxV", value<double>(&conf.volumeLimit)->default_value(200), "volume limit")
            ("parameters,p", value<string>(&parameters), "parameters to modify")
            ("values,v", value<string>(&values), "parameter values")
            ("grasps", value<string>(&conf.grasps_filename), "RW task file")
            ("save_grasps", value<string>(&conf.saved_grasps_filename), "saved RW task file")
            ("sigma_a", value<double>(&conf.sigma_a)->default_value(15), "grasp perturbation angle sigma")
            ("sigma_p", value<double>(&conf.sigma_p)->default_value(0.01), "grasp perturbation position sigma")
            ("ssamples", value<string>(&conf.ssamples), "surface samples file")
            ("seed", value<unsigned>(&conf.seed), "RNG seed");
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

    /* parse calculators */
    boost::tokenizer<> tok(indices);
    for (boost::tokenizer<>::iterator i = tok.begin(); i != tok.end(); ++i) {
        conf.index_calculators.push_back(*i);
    }

    /* parse parameters */
    boost::tokenizer<> ptok(parameters);
    for (boost::tokenizer<>::iterator i = ptok.begin(); i != ptok.end(); ++i) {
        conf.parameters.push_back(*i);
    }

    /* parse parameter values */
    istringstream sstr(values);
    double v;
    while (sstr >> v) {
        conf.values.push_back(v);
    }

    RW_ASSERT(conf.parameters.size() == conf.values.size());
    
    /* re-initialize rng */
    if (vm.count("seed")) {
        Math::seed(conf.seed);
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
    
    if (!config.ssamples.empty()) {
        INFO << "* Loading samples... ";
        data.ssamples = SurfaceSample::loadFromXML(config.ssamples);
        INFO << "Loaded." << endl;
    }
}

/******************************************************************************/
GripperEvaluator::Ptr make_evaluator(const Configuration& config) {
    GripperEvaluator::Ptr evaluator = ownedPtr(new GripperEvaluator());

    BOOST_FOREACH(const string& idx, config.index_calculators) {
        if (idx == "success") {
            QualityIndexCalculator::Ptr calc = ownedPtr(new SuccessIndexCalculator());
            evaluator->addQualityIndexCalculator(idx, calc);
        } else if (idx == "coverage") {
            QualityIndexCalculator::Ptr calc = ownedPtr(new CoverageIndexCalculator(config.covPosFilteringRadius, config.covAngleFilteringRadius));
            evaluator->addQualityIndexCalculator(idx, calc);
        } else if (idx == "alignment") {
            QualityIndexCalculator::Ptr calc = ownedPtr(new AlignmentIndexCalculator(config.aliFilteringRadius));
            evaluator->addQualityIndexCalculator(idx, calc);
        } else if (idx == "stress") {
            QualityIndexCalculator::Ptr calc = ownedPtr(new StressIndexCalculator(config.stressLimit));
            evaluator->addQualityIndexCalculator(idx, calc);
        } else if (idx == "volume") {
            QualityIndexCalculator::Ptr calc = ownedPtr(new VolumeIndexCalculator(config.volumeLimit));
            evaluator->addQualityIndexCalculator(idx, calc);
        } else if (idx == "wrench") {
            QualityIndexCalculator::Ptr calc = ownedPtr(new WrenchIndexCalculator());
            evaluator->addQualityIndexCalculator(idx, calc);
        }
    }

    return evaluator;
}

/******************************************************************************/
StandardGripperEvaluationProcessManager::Ptr make_evaluation_manager(const Configuration& config, const Data& data) {
    StandardGripperEvaluationProcessManager::Ptr manager = GripperEvaluationManagerFactory::makeStandardEvaluationManager(
                                                                                                                          data.td,
                                                                                                                          config.ngrasps,
                                                                                                                          config.threads,
                                                                                                                          data.ssamples
                                                                                                                          );

    if (!config.grasps_filename.empty()) {
        GraspSource::Ptr grasp_source = ownedPtr(new RWGraspDatabase(config.grasps_filename));
        GraspFilterChain::Ptr filter_chain = ownedPtr(new GraspFilterChain());
        GraspFilter::Ptr random_perturbation = ownedPtr(new RobustnessGraspFilter(config.ngrasps, config.sigma_p, Deg2Rad * config.sigma_a));
        filter_chain->addFilter(random_perturbation);
        grasp_source = ownedPtr(new FilteredGraspSource(grasp_source, filter_chain));
        manager->setGraspSource(grasp_source);
    }
    
    manager->setEvaluator(make_evaluator(config));

    return manager;
}

/******************************************************************************/
GripperQualityExtractor::Ptr make_extractor(const Configuration& config) {
    GripperQualityExtractor::Ptr extractor = ownedPtr(new IndexGripperQualityExtractor(config.index_calculators));

    return extractor;
}

/******************************************************************************/
void modify_parameters(Gripper::Ptr gripper, const Configuration& config) {
    Parametrized::Ptr parametrized = gripper.cast<Parametrized>();
    if (!parametrized) return;

    Parametrization::Ptr params = parametrized->getParametrization();

    for (unsigned i = 0; i < config.parameters.size(); ++i) {
        params->setParameter(config.parameters[i], config.values[i]);
    }
}

/******************************************************************************/
GripperQuality::Ptr evaluate_gripper(Gripper::Ptr gripper, GripperEvaluationProcessManager::Ptr manager, GripperQualityExtractor::Ptr extractor, CombineObjectives::Ptr method) {
    GripperQuality::Ptr quality = manager->evaluateGripper(gripper);

    vector<double> objectives = extractor->extract(quality);
    double Q = method->combine(objectives);
    quality->setIndex("Q", Q);

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

    modify_parameters(gripper, CONFIG);

    StandardGripperEvaluationProcessManager::Ptr manager = make_evaluation_manager(CONFIG, DATA);
    GripperQualityExtractor::Ptr extractor = make_extractor(CONFIG);
    CombineObjectives::Ptr method = CombineObjectivesFactory::make(CONFIG.method, CONFIG.weights);
    GripperQuality::Ptr quality = evaluate_gripper(gripper, manager, extractor, method);
    INFO << *quality << endl;
    GripperLoader::Ptr loader = new MasterGripperLoader();
    loader->save(CONFIG.result_filename, gripper);
    
    if (!CONFIG.saved_grasps_filename.empty()) {
        Grasps evaluated_grasps = manager->getSimulator()->getTasks();
        GraspTask::saveRWTask(evaluated_grasps, CONFIG.saved_grasps_filename);
    }

    return 0;
}