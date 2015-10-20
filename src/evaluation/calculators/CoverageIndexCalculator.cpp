/* 
 * File:   CoverageIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:30 AM
 */

#include "CoverageIndexCalculator.hpp"
#include <rwlibs/task/GraspTask.hpp>
#include <grasps/filters/GraspStatusFilter.hpp>
#include <grasps/filters/KDGraspFilter.hpp>
#include <grasps/GraspStatistics.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace gripperz::models;
using namespace gripperz::evaluation;
using namespace gripperz::grasps;
using namespace gripperz::context;
using namespace gripperz::grasps::filters;
using namespace gripperz::evaluation::calculators;
using namespace rwlibs::task;
using namespace rw::math;

CoverageIndexCalculator::CoverageIndexCalculator(double positionFilteringRadius, double angleFilteringRadius) :
_positionFilteringRadius(positionFilteringRadius),
_angleFilteringRadius(angleFilteringRadius) {

}

CoverageIndexCalculator::~CoverageIndexCalculator() {
}

QualityIndexValue CoverageIndexCalculator::calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) {
    DEBUG << "CALCULATING COVERAGE" << endl;
    double coverage = 0.0;

    /* split targets & samples*/
    GraspFilter::Ptr targetsFilter = new GraspStatusFilter({GraspResult::Success, GraspResult::Interference});

    Grasps targets = targetsFilter->filter(grasps);
    Grasps samples = grasps;

    /* filter targets & samples &/*/
    double R = 2.0 * sin(0.25 * _angleFilteringRadius);
    Q diff(7, _positionFilteringRadius, _positionFilteringRadius, _positionFilteringRadius, R, R, R, _angleFilteringRadius);
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
