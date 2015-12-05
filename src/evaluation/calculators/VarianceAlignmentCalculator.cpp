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
    
    double xs_dev = sqrt(Statistics<double>::meanAndVariance(xs).second);
    double ys_dev = sqrt(Statistics<double>::meanAndVariance(ys).second);
    double zs_dev = sqrt(Statistics<double>::meanAndVariance(zs).second);
    double rolls_dev = sqrt(Statistics<double>::meanAndVariance(rolls).second);
    double pitches_dev = sqrt(Statistics<double>::meanAndVariance(pitches).second);
    double yaws_dev = sqrt(Statistics<double>::meanAndVariance(yaws).second);
    
    DEBUG << "Calculating std devs..." << endl;
    DEBUG << " * X stddev = " << xs_dev << endl;
    DEBUG << " * Y stddev = " << ys_dev << endl;
    DEBUG << " * Z stddev = " << zs_dev << endl;
    DEBUG << " * ROLL stddev = " << rolls_dev * Rad2Deg << endl;
    DEBUG << " * PITCH stddev = " << pitches_dev * Rad2Deg << endl;
    DEBUG << " * YAW stddev = " << yaws_dev  * Rad2Deg<< endl;
    
    double pos_dev = (xs_dev + ys_dev + zs_dev) / 3.0;
    double angle_dev = (rolls_dev + pitches_dev + yaws_dev) / 3.0;
    
    DEBUG << "Calculating average stddevs..." << endl;
    DEBUG << " * average POSITION stddev = " << pos_dev << endl;
    DEBUG << " * average ANGLE stddev = " << angle_dev * Rad2Deg << endl;
    
    double pos_dev_expected = 2.0 * _positionUncertainty / sqrt(12.0);
    double angle_dev_expected = 2.0 * _angleUncertainty / sqrt(12.0);
    
    DEBUG << " * expected POSITION stddev = " << pos_dev_expected << endl;
    DEBUG << " * expected ANGLE stddev = " << angle_dev_expected * Rad2Deg << endl;
    
    double pos_alignment = 1.0 - pos_dev / pos_dev_expected;
    if (pos_alignment < 0.0) pos_alignment = 0.0;
    double angle_alignment = 1.0 - angle_dev / angle_dev_expected;
    if (angle_alignment < 0.0) angle_alignment = 0.0;
    
    DEBUG << "Calculating alignment..." << endl;
    DEBUG << " * POSITION alignment = " << pos_alignment << endl;
    DEBUG << " * ANGLE alignment = " << angle_alignment << endl;
    
    double alignment = (pos_alignment + angle_alignment) / 2.0;
    
    DEBUG << " * ALIGNMENT = " << alignment << endl;
    
    return alignment;
}

