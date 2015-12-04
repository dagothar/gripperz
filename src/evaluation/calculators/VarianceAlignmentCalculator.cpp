/* 
 * File:   VarianceAlignmentCalculator.cpp
 * Author: dagothar
 * 
 * Created on December 4, 2015, 4:17 PM
 */

#include "VarianceAlignmentCalculator.hpp"
#include <rw/math/Statistics.hpp>

#define DEBUG rw::common::Log::infoLog()
#define INFO rw::common::Log::infoLog()

using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;
using namespace gripperz::models;
using namespace gripperz::grasps;
using namespace rw::math;
using namespace rwlibs::task;
using namespace std;

VarianceAlignmentCalculator::VarianceAlignmentCalculator(double sigma_pos, double sigma_angle) :
_positionUncertainty(sigma_pos),
_angleUncertainty(sigma_angle) {

}

VarianceAlignmentCalculator::~VarianceAlignmentCalculator() {
}

QualityIndexValue VarianceAlignmentCalculator::calculate(Gripper::Ptr gripper, Grasps grasps) {
    vector<double> xs, ys, zs, rolls, pitches, yaws;
    
    typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget t, grasps->getAllTargets()) {

        // we take grasps with either success or interference
        if (
                t.second->getResult()->testStatus == GraspResult::Success
                || t.second->getResult()->testStatus == GraspResult::Interference
                ) {
            Transform3D<> pose = inverse(t.second->getResult()->objectTtcpLift);
            Vector3D<> p = pose.P();
            RPY<> r(pose.R());

            xs.push_back(p[0]);
            ys.push_back(p[1]);
            zs.push_back(p[2]);
            rolls.push_back(r[0]);
            pitches.push_back(r[1]);
            yaws.push_back(r[2]);
        }
    }
    
    double xs_var = Statistics<double>::meanAndVariance(xs).second;
    double ys_var = Statistics<double>::meanAndVariance(ys).second;
    double zs_var = Statistics<double>::meanAndVariance(zs).second;
    double rolls_var = Statistics<double>::meanAndVariance(rolls).second;
    double pitches_var = Statistics<double>::meanAndVariance(pitches).second;
    double yaws_var = Statistics<double>::meanAndVariance(yaws).second;
    
    DEBUG << "Calculating variances..." << endl;
    DEBUG << " * X variance = " << xs_var << endl;
    DEBUG << " * Y variance = " << ys_var << endl;
    DEBUG << " * Z variance = " << zs_var << endl;
    DEBUG << " * ROLL variance = " << rolls_var << endl;
    DEBUG << " * PITCH variance = " << pitches_var << endl;
    DEBUG << " * YAW variance = " << yaws_var << endl;
    
    double pos_var = (xs_var + ys_var + zs_var) / 3.0;
    double angle_var = (rolls_var + pitches_var + yaws_var) / 3.0;
    
    DEBUG << "Calculating average variances..." << endl;
    DEBUG << " * average POSITION variance = " << pos_var << endl;
    DEBUG << " * average ANGLE variance = " << angle_var << endl;
    
    double pos_alignment = 1.0 - pos_var / _positionUncertainty;
    if (pos_alignment < 0.0) pos_alignment = 0.0;
    double angle_alignment = 1.0 - angle_var / _angleUncertainty;
    if (angle_alignment < 0.0) angle_alignment = 0.0;
    
    DEBUG << "Calculating alignment..." << endl;
    DEBUG << " * POSITION alignment = " << pos_alignment << endl;
    DEBUG << " * ANGLE alignment = " << angle_alignment << endl;
    
    double alignment = (pos_alignment + angle_alignment) / 2.0;
    
    DEBUG << " * ALIGNMENT = " << alignment << endl;
    
    return alignment;
}

