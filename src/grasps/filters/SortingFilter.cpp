/* 
 * File:   SortingFilter.cpp
 * Author: dagothar
 * 
 * Created on October 26, 2015, 1:56 PM
 */

#include "SortingFilter.hpp"
#include <algorithm>

using namespace std;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::task;

SortingFilter::SortingFilter(Compare::Ptr comp) : _comp(comp) {
}

SortingFilter::~SortingFilter() {
}

struct Proxy {

    Proxy(SortingFilter::Compare::Ptr comp) : comp(comp) {
    }

    bool operator()(rwlibs::task::GraspTarget t1, rwlibs::task::GraspTarget t2) {
        return comp->operator()(t1, t2);
    }

    SortingFilter::Compare::Ptr comp;
};

Grasps SortingFilter::filter(Grasps grasps) {
    Grasps filteredGrasps = grasps->clone();

    BOOST_FOREACH(GraspSubTask& subtask, grasps->getSubTasks()) {

        GraspSubTask subtask_copy = subtask.clone();

        std::vector<GraspTarget> subtask_targets = subtask.getTargets();
        std::sort(subtask_targets.begin(), subtask_targets.end(), Proxy(_comp));

        BOOST_FOREACH(const GraspTarget& target, subtask_targets) {

            subtask_copy.addTarget(target);
        }

        filteredGrasps->addSubTask(subtask_copy);
    }

    return filteredGrasps;
}

