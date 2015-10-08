/* 
 * File:   KDGraspFilter.cpp
 * Author: dagothar
 * 
 * Created on October 8, 2015, 2:57 PM
 */

#include "KDGraspFilter.hpp"
#include <rwlibs/algorithms/kdtree/KDTree.hpp>
#include <rwlibs/algorithms/kdtree/KDTreeQ.hpp>

using namespace std;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace rwlibs::algorithms;
using namespace rwlibs::task;
using namespace rw::math;

KDGraspFilter::KDGraspFilter(const rw::math::Q& filteringBox) {
    _filteringBox = filteringBox;
}

KDGraspFilter::~KDGraspFilter() {
}

Grasps KDGraspFilter::filter(Grasps grasps) {
    if (!grasps) {
        return NULL;
    }

    Grasps filteredGrasps = copyGrasps(grasps);

    // create nodes for succesful grasps
    typedef GraspResult::Ptr ValueType;
    typedef KDTreeQ<ValueType> NNSearch;
    vector<NNSearch::KDNode> nodes;

    int nTasks = 0;

    typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget p, filteredGrasps->getAllTargets()) {

        if (p.second->getResult()->testStatus == GraspResult::Success ||
                p.second->getResult()->testStatus == GraspResult::Interference ||
                p.second->getResult()->testStatus == GraspResult::ObjectSlipped ||
                p.second->getResult()->testStatus == GraspResult::UnInitialized) {

            Q key(7);
            key[0] = p.second->pose.P()[0];
            key[1] = p.second->pose.P()[1];
            key[2] = p.second->pose.P()[2];
            EAA<> eaa(p.second->pose.R());
            key[3] = eaa.axis()(0);
            key[4] = eaa.axis()(1);
            key[5] = eaa.axis()(2);
            key[6] = eaa.angle();

            nodes.push_back(NNSearch::KDNode(key, p.second->getResult()));

            ++nTasks;
        }
    }

    NNSearch *nntree = NNSearch::buildTree(nodes);
    std::list<const NNSearch::KDNode*> result;

    int nRemoved = 0;

    BOOST_FOREACH(NNSearch::KDNode& node, nodes) {
        if (node.value->testStatus != GraspResult::Filtered) {
            result.clear();
            Q key = node.key;
            nntree->nnSearchRect(key - _filteringBox, key + _filteringBox, result);

            int removed = 0;

            BOOST_FOREACH(const NNSearch::KDNode* n, result) {
                if (n->key == node.key) continue;

                if (n->value->testStatus != GraspResult::Filtered) ++removed;
                const_cast<NNSearch::KDNode*> (n)->value->testStatus = GraspResult::Filtered; // this is a hack
            }
            nRemoved += removed;
        }
    }

    return filteredGrasps;
}