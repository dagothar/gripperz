/**
 * @file GripperEvaluator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "OldGripperEvaluator.hpp"

#include <algorithm>
#include <grasps/GraspStatistics.hpp>
#include <grasps/GraspSource.hpp>
#include <grasps/filters/KDGraspFilter.hpp>
#include <grasps/filters/GraspStatusFilter.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <rwlibs/task/GraspTarget.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace gripperz::context;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;

OldGripperEvaluator::OldGripperEvaluator(TaskDescription::Ptr context, AlignmentCalculator::Ptr alignmentCalculator) :
_context(context),
_alignmentCalculator(alignmentCalculator) {
}

OldGripperEvaluator::~OldGripperEvaluator() {
}

bool OldGripperEvaluator::isSane(models::OldGripper::Ptr gripper) {
    if (!gripper->isSane()) return false;

    return true;
}

GripperQuality OldGripperEvaluator::evaluate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {

    GripperQuality quality;
    
    quality["success"] = calculateSuccess(gripper, grasps);
    quality["robustness"] = calculateRobustness(gripper, grasps, NULL);
    quality["alignment"] = calculateAlignment(grasps);
    quality["coverage"] = calculateCoverage(gripper, grasps);
    quality["wrench"] = calculateWrench(gripper, grasps);
    quality["stress"] = calculateStress(gripper);
    quality["volume"] = calculateVolume(gripper);
    
    return quality;
}


double OldGripperEvaluator::calculateSuccess(models::OldGripper::Ptr gripper, Grasps grasps) {
    DEBUG << "CALCULATING SUCCESS - " << endl;
    std::vector<std::pair<class GraspSubTask*, class GraspTarget*> > allTargets = grasps->getAllTargets();
    
    int nAllTargets = allTargets.size();
    int successes = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Success);
    int filtered = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Filtered);
    int failures = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::SimulationFailure);
    int samples = GraspStatistics::countGraspsWithStatus(grasps, GraspResult::Skip);

    DEBUG << "alltargets= " << nAllTargets << endl;
    DEBUG << "successes= " << successes << endl;
    DEBUG << "filtered= " << filtered << endl;
    DEBUG << "failures= " << failures << endl;
    DEBUG << "samples= " << samples << endl;

    double validTasks = nAllTargets - filtered - failures - samples;
    
    if (validTasks == 0) {
        RW_WARN("No valid tasks");
        return 0.0;
    }

    double successIndex = 1.0 * successes / validTasks;

    return successIndex;
}

double OldGripperEvaluator::calculateRobustness(models::OldGripper::Ptr gripper, Grasps grasps, Grasps rgrasps) {
    if (rgrasps == NULL) {
        RW_WARN("Not calculating robustness.");
        return 1.0;
    }

    DEBUG << "CALCULATING ROBUSTNESS - " << endl;
    std::vector<std::pair<class GraspSubTask*, class GraspTarget*> > allTargets = rgrasps->getAllTargets();
    
    int nAllTargets = allTargets.size();
    int successes = GraspStatistics::countGraspsWithStatus(rgrasps, GraspResult::Success);

    DEBUG << "alltargets= " << nAllTargets << endl;
    DEBUG << "successes= " << successes << endl;

    double validTasks = nAllTargets;
    if (validTasks == 0) {
        RW_WARN("No valid tasks");
        return 0.0;
    }

    double robustnessIndex = 1.0 * successes / validTasks;

    return robustnessIndex;
}

double OldGripperEvaluator::calculateCoverage(models::OldGripper::Ptr gripper, Grasps grasps) {
    DEBUG << "CALCULATING COVERAGE - " << endl;
    double coverage = 0.0;
    
    /* split targets & samples*/
    GraspFilter::Ptr targetsFilter = new GraspStatusFilter({GraspResult::Success, GraspResult::Interference});
    
    Grasps targets = targetsFilter->filter(grasps);
    Grasps samples = grasps;

    /* filter targets & samples &/*/
    Q covDist = _context->getCoverageDistance();
    double R = 2.0 * sin(0.25 * covDist(1));
    Q diff(7, covDist(0), covDist(0), covDist(0), R, R, R, covDist(2));
    GraspFilter::Ptr coverageFilter = new KDGraspFilter(diff);
    
    Grasps coverageTargets = coverageFilter->filter(targets);
    Grasps coverageSamples = coverageFilter->filter(samples);

    int okTargets = GraspStatistics::countGraspTargets(coverageTargets);
    int allTargets = GraspStatistics::countGraspTargets(coverageSamples);
    if (allTargets == 0) {
        return 0.0;
    }

    coverage = 1.0 * okTargets / (allTargets * 1.0);

    return coverage;
}

double OldGripperEvaluator::calculateAlignment(rwlibs::task::GraspTask::Ptr tasks) {
    DEBUG << "CALCULATING ALIGNMENT - " << endl;

    double alignment = _alignmentCalculator->calculateAlignment(tasks);

    DEBUG << "Alignment index= " << alignment << endl;

    return alignment;
}

bool sortf(double a, double b) {
    return (a > b);
}

double OldGripperEvaluator::calculateWrench(models::OldGripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
    DEBUG << "CALCULATING WRENCH - " << endl;

    vector<double> wrenches; // used to find the top 10%
    Q wrench(3, 0, 0, 0);

    int successes = 0;
    typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget p, tasks->getAllTargets()) {

        if (
                p.second->getResult()->testStatus == GraspResult::Success
                || p.second->getResult()->testStatus == GraspResult::Interference
                ) {
            successes++;

            Q result = p.second->getResult()->qualityAfterLifting;

            wrench(0) += result(0);
            wrench(1) += result(1);

            wrenches.push_back(result(1));
        }
    }

    // find top 20%
    sort(wrenches.begin(), wrenches.end(), sortf);

    int num = 0.2 * successes < 1 ? 1 : 0.2 * successes;

    if (wrenches.size() > 0) {
        for (int i = 0; i < num; ++i) {
            wrench(2) += wrenches[i];
        }
    }

    // calculate averages
    if (successes == 0) {
        successes = 1;
    }

    wrench(0) /= successes;
    wrench(1) /= successes;
    wrench(2) /= num;

    return wrench(0);
}

double OldGripperEvaluator::calculateTopWrench(models::OldGripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks) {
    DEBUG << "CALCULATING TOPWRENCH - " << endl;

    vector<double> wrenches; // used to find the top 10%
    Q wrench(3, 0, 0, 0);

    int successes = 0;
    typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget p, tasks->getAllTargets()) {

        if (
                p.second->getResult()->testStatus == GraspResult::Success
                || p.second->getResult()->testStatus == GraspResult::Interference
                ) {
            successes++;

            Q result = p.second->getResult()->qualityAfterLifting;

            wrench(0) += result(0);
            wrench(1) += result(1);

            wrenches.push_back(result(1));
        }
    }

    // find top 20%
    sort(wrenches.begin(), wrenches.end(), sortf);

    int num = 0.2 * successes < 1 ? 1 : 0.2 * successes;

    if (wrenches.size() > 0) {
        for (int i = 0; i < num; ++i) {
            wrench(2) += wrenches[i];
        }
    }

    // calculate averages
    if (successes == 0) {
        successes = 1;
    }

    wrench(0) /= successes;
    wrench(1) /= successes;
    wrench(2) /= num;

    return wrench(2);
}

double OldGripperEvaluator::calculateStress(models::OldGripper::Ptr gripper) {
    DEBUG << "CALCULATING STRESS - " << endl;
    double maxstress = gripper->getMaxStress();
    DEBUG << "Gripper stress= " << maxstress << endl;

    double stress = 1.0 - (maxstress / _context->getStressLimit());

    if (stress < 0.0) {
        stress = 0.0;
    }

    return stress;
}

double OldGripperEvaluator::calculateVolume(models::OldGripper::Ptr gripper) {
    DEBUG << "CALCULATING VOLUME - " << endl;
    double gripperVolume = gripper->getVolume();
    DEBUG << "Gripper volume= " << gripperVolume << endl;

    double volumeIndex = 1.0 - gripperVolume / _context->getVolumeLimit();

    if (volumeIndex < 0.0) {
        volumeIndex = 0.0;
    }

    return volumeIndex;
}
