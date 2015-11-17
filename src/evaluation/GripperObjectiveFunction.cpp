/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "GripperObjectiveFunction.hpp"

#include <rwlibs/task/GraspTask.hpp>
#include <grasps/GraspSource.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::process;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::grasps;
using namespace gripperz::math;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;

GripperObjectiveFunction::GripperObjectiveFunction(GripperBuilder::Ptr builder, GripperEvaluationProcessManager::Ptr manager, GripperQualityExtractor::Ptr extractor) :
_builder(builder),
_manager(manager),
_extractor(extractor) {
}

GripperObjectiveFunction::~GripperObjectiveFunction() {
}

std::vector<double> GripperObjectiveFunction::evaluate(const std::vector<double>& x) {
    Vector results;

    /*
     * Build gripper.
     */
    Gripper::Ptr gripper = NULL;
    try {

        gripper = _builder->vectorToGripper(x);
        _lastGripper = gripper;

    } catch (const std::exception& e) {
        RW_THROW("Exception during gripper generation! " << e.what());
    }

    /*
     * Evaluate gripper.
     */
    RW_ASSERT(gripper != NULL);
    try {

        GripperQuality::Ptr q = _manager->evaluateGripper(gripper);

        _lastQuality = q;
        _lastGripper->setQuality(q);

        results = _extractor->extract(q);
    } catch (const std::exception& e) {
        RW_THROW("Exception during gripper evaluation! " << e.what());
    }

    /*
     * Callback (if exists).
     */
    if (_callback) {
        _callback(_lastGripper);
    }

    return results;
}
