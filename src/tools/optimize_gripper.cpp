/* 
 * File:   optimize_gripper.cpp
 * Author: dagothar
 * 
 * Created on November 17, 2015, 1:46 PM
 */

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
#include <grasps/planners/BasicParallelGripperGraspPlanner.hpp>
#include <optimization/OptimizationManager.hpp>
#include <optimization/OptimizerFactory.hpp>
#include <evaluation/GripperObjectiveFunction.hpp>
#include <math/Types.hpp>
#include <math/CombinedFunction.hpp>
#include <boost/filesystem.hpp>

#include "models/PrototypeGripperBuilder.hpp"
#include "parametrization/VectorParametrizationTranslator.hpp"
#include "evaluation/calculators/VarianceAlignmentCalculator.hpp"

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
using namespace boost::filesystem;
using namespace gripperz::loaders;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace gripperz::context;
using namespace gripperz::process;
using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace gripperz::grasps::filters;
using namespace gripperz::grasps::databases;
using namespace gripperz::grasps::decorators;
using namespace gripperz::math;
using namespace gripperz::optimization;
using namespace gripperz::parametrization;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

/******************************************************************************/
ofstream log_file;

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
    vector<double> limits_low;
    vector<double> limits_high;
    string grasps_filename;
    string saved_grasps_filename;
    string ssamples;
    string optimizer;
    vector<string> filters;

    double covPosFilteringRadius;
    double covAngleFilteringRadius;
    double aliPositionFilteringRadius;
    double aliAngleFilteringRadius;
    double stressLimit;
    double volumeLimit;
    double sigma_p;
    double sigma_a;
    double rho0, rhof;
    int maxfev;
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
    string offset;
    string indices;
    string filters;
    string parameters;
    string limits_low;
    string limits_high;

    options_description desc("Options");
    desc.add_options()
            ("help", "help message")
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
            ("aliP", value<double>(&conf.aliPositionFilteringRadius)->default_value(0.003), "position uncertainty for alignment")
            ("aliA", value<double>(&conf.aliAngleFilteringRadius)->default_value(5), "angle uncertainty for alignment")
            ("maxS", value<double>(&conf.stressLimit)->default_value(10), "stress limit")
            ("maxV", value<double>(&conf.volumeLimit)->default_value(200), "volume limit")
            ("parameters,p", value<string>(&parameters), "parameters to optimize")
            ("low,l", value<string>(&limits_low)->required(), "low limits for parameters")
            ("high,h", value<string>(&limits_high)->required(), "high limits for parameters")
            ("grasps", value<string>(&conf.grasps_filename), "RW task file")
            ("filters,f", value<string>(&filters), "grasp filters to apply")
            ("save_grasps", value<string>(&conf.saved_grasps_filename), "saved RW task file")
            ("sigma_a", value<double>(&conf.sigma_a)->default_value(15), "grasp perturbation angle sigma")
            ("sigma_p", value<double>(&conf.sigma_p)->default_value(0.01), "grasp perturbation position sigma")
            ("optimizer", value<string>(&conf.optimizer)->default_value("simplex"), "optimization method (simplex, coord, bobyqa, simu, powell)")
            ("rho0", value<double>(&conf.rho0)->default_value(0.1), "initial step size")
            ("rhof", value<double>(&conf.rhof)->default_value(0.001), "final step size")
            ("maxfev", value<int>(&conf.maxfev)->default_value(1e2), "max number of function evaluations")
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
        cout << e.what() << endl;
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

    /* parse filters */
    boost::tokenizer<> filters_tok(filters);
    for (boost::tokenizer<>::iterator i = filters_tok.begin(); i != filters_tok.end(); ++i) {
        conf.filters.push_back(*i);
    }

    /* parse parameters */
    boost::tokenizer<> ptok(parameters);
    for (boost::tokenizer<>::iterator i = ptok.begin(); i != ptok.end(); ++i) {
        conf.parameters.push_back(*i);
    }

    /* parse limit values */
    istringstream low_sstr(limits_low);
    double v;
    while (low_sstr >> v) {
        conf.limits_low.push_back(v);
    }

    istringstream high_sstr(limits_high);
    while (high_sstr >> v) {
        conf.limits_high.push_back(v);
    }

    RW_ASSERT(conf.parameters.size() == conf.limits_low.size());
    RW_ASSERT(conf.parameters.size() == conf.limits_high.size());

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
            //QualityIndexCalculator::Ptr calc = ownedPtr(new AlignmentIndexCalculator(config.aliFilteringRadius));
            QualityIndexCalculator::Ptr calc = ownedPtr(new VarianceAlignmentCalculator(config.aliPositionFilteringRadius, Deg2Rad * config.aliAngleFilteringRadius));
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
        } else if (idx == "significance") {
            QualityIndexCalculator::Ptr calc = ownedPtr(new SignificanceIndexCalculator());
            evaluator->addQualityIndexCalculator(idx, calc);
        }
    }

    return evaluator;
}

/******************************************************************************/
GraspSource::Ptr make_grasp_source(const Configuration& config, const Data& data) {
    GraspSource::Ptr source;

    /* construct basic source */
    if (!config.grasps_filename.empty()) {
        INFO << "Grasp source: GRASPS" << endl;

        source = ownedPtr(new RWGraspDatabase(config.grasps_filename));

    } else {
        INFO << "Grasp source: PLANNER" << endl;

        BasicParallelGripperGraspPlanner::Ptr planner = ownedPtr(new BasicParallelGripperGraspPlanner(config.ngrasps, data.td->getInitState(), data.td));
        planner->setSurfaceSamples(data.ssamples);

        source = planner;
    }

    /* apply filters */
    GraspFilterChain::Ptr filter_chain = ownedPtr(new GraspFilterChain());
    
    filter_chain->addFilter(ownedPtr(new ClearStatusFilter()));

    BOOST_FOREACH(const string& f, config.filters) {
        if (f == "perturb") {
            GraspFilter::Ptr filter = ownedPtr(new RobustnessGraspFilter(config.ngrasps, config.sigma_p, Deg2Rad * config.sigma_a));
            filter_chain->addFilter(filter);
        }
    }

    source = ownedPtr(new FilteredGraspSource(source, filter_chain));

    return source;
}

/******************************************************************************/
StandardGripperEvaluationProcessManager::Ptr make_evaluation_manager(const Configuration& config, const Data& data) {

    StandardGripperEvaluationProcessManager::Ptr manager =
            GripperEvaluationManagerFactory::makeStandardEvaluationManager(
                                                                           data.td,
                                                                           config.ngrasps,
                                                                           config.threads,
                                                                           data.ssamples
                                                                           );

    manager->setGraspSource(make_grasp_source(config, data));
    manager->setEvaluator(make_evaluator(config));

    return manager;
}

/******************************************************************************/
GripperQualityExtractor::Ptr make_extractor(const Configuration& config) {
    GripperQualityExtractor::Ptr extractor = ownedPtr(new IndexGripperQualityExtractor(config.index_calculators));

    return extractor;
}

/******************************************************************************/
OptimizationManager::Ptr make_optimization_manager(const Configuration& config) {
    Optimizer::Ptr optimizer;
    if (config.optimizer == "simplex") {
        optimizer = OptimizerFactory::makeSimplexOptimizer(config.rho0, config.rhof, config.maxfev);
    } else if (config.optimizer == "coord") {
        optimizer = OptimizerFactory::makeCoordinateDescentOptimizer(config.rho0, config.rhof, config.maxfev);
    } else if (config.optimizer == "bobyqa") {
        optimizer = OptimizerFactory::makeBOBYQAOptimizer(config.parameters.size(), config.rho0, config.rhof, config.maxfev);
    } else if (config.optimizer == "simu") {
        optimizer = OptimizerFactory::makeSimulatedAnnealingOptimizer(config.rho0, config.rhof, config.maxfev);
    } else if (config.optimizer == "powell") {
        optimizer = OptimizerFactory::makePowellOptimizer(config.rho0, config.rhof, config.maxfev);
    } else if (config.optimizer == "rbf") {
        optimizer = OptimizerFactory::makeRbfOptimizer(250, config.maxfev);
    } else {
        RW_THROW("Optimizer not supported!");
    }

    RangeList limits;
    for (unsigned i = 0; i < config.parameters.size(); ++i) {
        limits.push_back(make_pair(config.limits_low[i], config.limits_high[i]));
    }

    OptimizationManager::Ptr optimization_manager = ownedPtr(new OptimizationManager(optimizer, limits, true));
    return optimization_manager;
}

/******************************************************************************/
GripperBuilder::Ptr make_gripper_builder(const Configuration& config, Data& data) {
    ParametrizedGripper::Ptr gripper = data.gripper.cast<ParametrizedGripper>();
    RW_ASSERT(gripper != NULL);
    
    ParametrizationTranslator::Ptr translator = ownedPtr(new VectorParametrizationTranslator(config.parameters));
    
    GripperBuilder::Ptr builder = ownedPtr(new PrototypeGripperBuilder(gripper, translator));
    return builder;
}

/******************************************************************************/
void callback(Gripper::Ptr gripper1, GripperQualityExtractor::Ptr extractor, CombineObjectives::Ptr combiner, GripperBuilder::Ptr builder) {
    ParametrizedGripper::Ptr gripper = gripper1.cast<ParametrizedGripper>();
    RW_ASSERT(gripper != NULL);
    
    static unsigned step = 0;
    
    Vector x = builder->gripperToVector(gripper);
    GripperQuality::Ptr quality = gripper->getQuality();
    Vector r = extractor->extract(quality);
    double q = combiner->combine(r);
    ++step;
    
    log_file << step << ", ";

    for (unsigned i = 0; i < x.size(); ++i) {
        cout << x[i] << ", ";
        log_file <<  x[i] << ", ";
    }
    
    for (unsigned i = 0; i < r.size(); ++i) {
        cout << r[i] << ", ";
        log_file <<  r[i] << ", ";
    }
    
    cout << q << endl;
    log_file << q <<  endl;
    
    stringstream sstr;
    sstr << "step_" << step << ".grp.xml";
    path gripper_file = path(sstr.str());
    GripperLoader::Ptr loader = new MasterGripperLoader();
    loader->save(gripper_file.string(), gripper);
}

/******************************************************************************/
ObjectiveFunction::Ptr make_objective_function(const Configuration& config, Data& data, GripperEvaluationProcessManager::Ptr evaluation_manager, GripperBuilder::Ptr builder) {    
    GripperQualityExtractor::Ptr extractor = make_extractor(config);
    
    GripperObjectiveFunction::Ptr multi_function = new GripperObjectiveFunction(builder, evaluation_manager, extractor);
    
    CombineObjectives::Ptr comb_method = CombineObjectivesFactory::make(config.method, config.weights);
    
    multi_function->setCallback(boost::bind<>(&callback, _1, extractor, comb_method, builder));
    
    ObjectiveFunction::Ptr objective = new CombinedFunction(multi_function, comb_method);
    
    return objective;
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
    
    log_file.open(path("log.csv").string());
    log_file << "step, ";
    for (unsigned i = 0; i < CONFIG.parameters.size(); ++i) {
        log_file << CONFIG.parameters[i] << ", ";
    }
    for (unsigned i = 0; i < CONFIG.index_calculators.size(); ++i) {
        log_file << CONFIG.index_calculators[i] << ", ";
    }
    log_file << "Q" << endl;
    
    ParametrizedGripper::Ptr gripper = DATA.gripper.cast<ParametrizedGripper>();
    RW_ASSERT(gripper != NULL);

    GripperBuilder::Ptr builder = make_gripper_builder(CONFIG, DATA);
    GripperEvaluationProcessManager::Ptr evaluation_manager = make_evaluation_manager(CONFIG, DATA);
    ObjectiveFunction::Ptr objective = make_objective_function(CONFIG, DATA, evaluation_manager, builder);
    OptimizationManager::Ptr optimization_manager = make_optimization_manager(CONFIG);
    
    Vector guess = builder->gripperToVector(gripper);
    cout << "Initial guess: " << guess << endl;
    
    Vector result = optimization_manager->optimize(objective, guess, "maximize");
    
    Gripper::Ptr opt_gripper = builder->vectorToGripper(result);
    GripperQuality::Ptr opt_gripper_q = evaluation_manager->evaluateGripper(opt_gripper);
    opt_gripper->setQuality(opt_gripper_q);

    cout << "Optimization successful!" << endl;
    cout << "Optimal values for parameters:" << result << endl;

    /* save results */
    path opt_gripper_file = path(CONFIG.result_filename);
    GripperLoader::Ptr loader = new MasterGripperLoader();
    loader->save(opt_gripper_file.string(), opt_gripper);
    
    log_file.close();

    return 0;
}