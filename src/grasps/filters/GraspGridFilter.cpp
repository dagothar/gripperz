/* 
 * File:   GraspGridFilter.cpp
 * Author: dagothar
 * 
 * Created on October 24, 2015, 8:21 PM
 */

#include "GraspGridFilter.hpp"

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
            BOOST_FOREACH(const GraspTarget& new_target, grid) {
                subtask_copy.addTarget(new_target);
            }    
        }

        filteredGrasps->addSubTask(subtask_copy);
    }

    return filteredGrasps;
}

vector<GraspTarget> GraspGridFilter::makeGrid(const GraspTarget& seed) const {
    vector<GraspTarget> grid;
    

    return grid;
}
