/* 
 * File:   GraspGridFilter.cpp
 * Author: dagothar
 * 
 * Created on October 24, 2015, 8:21 PM
 */

#include "GraspGridFilter.hpp"

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::task;
using namespace rw::math;

GraspGridFilter::GraspGridFilter(const std::vector<double>& min, const std::vector<double>& max, const std::vector<int>& res) :
_min(min),
_max(max),
_res(res) {

}

GraspGridFilter::~GraspGridFilter() {
}

Grasps GraspGridFilter::filter(Grasps grasps) {
    Grasps filteredGrasps = grasps->clone();

    BOOST_FOREACH(GraspSubTask& subtask, grasps->getSubTasks()) {

        GraspSubTask subtask_copy = subtask.clone();

        BOOST_FOREACH(const GraspTarget& target, subtask.getTargets()) {

            vector<GraspTarget> grid = makeGrid(target);
            
            DEBUG << "grid.size = " << grid.size() << endl;

            BOOST_FOREACH(const GraspTarget& new_target, grid) {
                subtask_copy.addTarget(new_target);
            }
        }

        filteredGrasps->addSubTask(subtask_copy);
    }

    return filteredGrasps;
}

vector<GraspTarget> GraspGridFilter::makeGrid(const GraspTarget& seed) const {
    RW_ASSERT(_min.size() == 6);
    RW_ASSERT(_max.size() == 6);
    RW_ASSERT(_res.size() == 6);

    vector<GraspTarget> grid{seed};

    for (unsigned i = 0; i < 6; ++i) {
        grid = makeOneDimension(grid, i);
        
        DEBUG << "grid1D.size = " << grid.size() << endl;
    }

    return grid;
}

Transform3D<> getTransform(unsigned i, double xi) {
    Transform3D<> t;

    switch (i) {
        case 0:
            t = Transform3D<>(Vector3D<>(xi, 0, 0));
            break;
        case 1:
            t = Transform3D<>(Vector3D<>(0, xi, 0));
            break;
        case 2:
            t = Transform3D<>(Vector3D<>(0, 0, xi));
            break;
        case 3:
            t = Transform3D<>(RPY<>(xi, 0, 0).toRotation3D());
            break;
        case 4:
            t = Transform3D<>(RPY<>(0, xi, 0).toRotation3D());
            break;
        case 5:
            t = Transform3D<>(RPY<>(0, 0, xi).toRotation3D());
            break;
        default:
            break;
    }

    return t;
}

vector<GraspTarget> GraspGridFilter::makeOneDimension(const vector<GraspTarget>& seeds, unsigned i) const {
    RW_ASSERT(i >= 0 && i < 6);

    if (_res[i] <= 1) {
        DEBUG << "skipping dimension " << i << endl;
        return seeds;
    }

    vector<GraspTarget> targets;

    BOOST_FOREACH(const GraspTarget& seed, seeds) {
        
        double dxi = (_max[i] - _min[i]) / (_res[i] - 1);
        
        DEBUG << "xi0 = " << _min[i] << endl;
        DEBUG << "xi1 = " << _max[i] << endl;
        DEBUG << "dxi = " << dxi << endl;
        
        for (double xi = _min[i]; xi <= _max[i] + 1e-6; xi += dxi) {

            Transform3D<> new_target = seed.pose * getTransform(i, xi);
            targets.push_back(new_target);
        }
    }

    return targets;
}
