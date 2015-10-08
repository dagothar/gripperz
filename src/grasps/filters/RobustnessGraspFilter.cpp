/* 
 * File:   RobustnessGraspFilter.cpp
 * Author: dagothar
 * 
 * Created on October 8, 2015, 3:28 PM
 */

#include "RobustnessGraspFilter.hpp"

using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::task;
using namespace rw::math;

RobustnessGraspFilter::RobustnessGraspFilter(unsigned nOfGrasps, double sigmaPos, double sigmaAngle) :
_numberOfGrasps(nOfGrasps),
_positionUncertainty(sigmaPos),
_angleUncertainty(sigmaAngle) {
}

RobustnessGraspFilter::~RobustnessGraspFilter() {
}

Grasps RobustnessGraspFilter::filter(Grasps grasps) {

    /* prepare task */
    auto targets = grasps->getAllTargets();
    int n_targets = targets.size();
    if (n_targets == 0) {
        RW_WARN("No grasps to perturb");
        return grasps->clone();
    }

    GraspTask::Ptr perturbed = grasps->clone();
    GraspSubTask stask = grasps->getSubTasks()[0].clone();

    /* generate perturbed targets */
    unsigned generated = 0;
    while (generated++ < _numberOfGrasps) {
        /* pick random target */
        int idx = Math::ranI(0, n_targets);
        GraspTarget* target = targets[idx].second;

        Vector3D<> pos(Math::ranNormalDist(0, _positionUncertainty), Math::ranNormalDist(0, _positionUncertainty), Math::ranNormalDist(0, _positionUncertainty));

        EAA<> rot(Math::ranNormalDist(0, _angleUncertainty), Math::ranNormalDist(0, _angleUncertainty), Math::ranNormalDist(0, _angleUncertainty));

        Transform3D<> new_target = target->pose * Transform3D<>(pos, rot);
        stask.addTarget(new_target);
    }

    perturbed->addSubTask(stask);

    return perturbed;
}