/* 
 * File:   AlignmentIndexCalculator.cpp
 * Author: dagothar
 * 
 * Created on October 13, 2015, 11:33 AM
 */

#include "AlignmentIndexCalculator.hpp"
#include <rwlibs/algorithms/kdtree/KDTree.hpp>
#include <rwlibs/algorithms/kdtree/KDTreeQ.hpp>


#define DEBUG rw::common::Log::infoLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace rw::math;
using namespace rw::common;
using namespace rwlibs::task;
using namespace rwlibs::algorithms;
using namespace gripperz::models;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;

AlignmentIndexCalculator::AlignmentIndexCalculator(double positionFilteringRadius, double angleFilteringRadius) :
_positionFilteringRadius(positionFilteringRadius),
_angleFilteringRadius(angleFilteringRadius) {

}

AlignmentIndexCalculator::~AlignmentIndexCalculator() {
}

vector<Vector3D<> > getVersors(const Vector3D<>& axis, const vector<Transform3D<> > transforms) {
    vector<Vector3D<> > versors;

    BOOST_FOREACH(const Transform3D<>& t, transforms) {
        Vector3D<> v = t.R() * axis;
        versors.push_back(v);
    }

    return versors;
}

struct Point {
    typedef rw::common::Ptr<Point> Ptr;

    Vector3D<> point;
    bool filtered;
    vector<Point*> neighbours;

    Point(const Vector3D<> v) :
    point(v),
    filtered(false) {
    }
};

bool pointSort(const KDTreeQ<Point::Ptr>::KDNode& n1, const KDTreeQ<Point::Ptr>::KDNode& n2) {
    return n1.value->neighbours.size() > n2.value->neighbours.size();
}

vector<Vector3D<> > filterPoints(const vector<Vector3D<> >& points, double radius = 0.1) {
    /* build search tree */
    typedef Point::Ptr ValueType;
    typedef KDTreeQ<ValueType> NNSearch;
    vector<NNSearch::KDNode> nodes;

    BOOST_FOREACH(const Vector3D<>& p, points) {
        Q key(3);
        key[0] = p[0];
        key[1] = p[1];
        key[2] = p[2];

        nodes.push_back(NNSearch::KDNode(key, ownedPtr(new Point(p))));
    }
    NNSearch *nntree = NNSearch::buildTree(nodes);

    /* find point neighbours */
    std::list<const NNSearch::KDNode*> result;
    Q diff(3, radius, radius, radius);

    BOOST_FOREACH(NNSearch::KDNode& node, nodes) {
        result.clear();
        Q key = node.key;
        nntree->nnSearchRect(key - diff, key + diff, result);

        BOOST_FOREACH(const NNSearch::KDNode* n, result) {
            if (n->key == node.key) continue;

            //const_cast<NNSearch::KDNode*>(n)->value->filtered = true;
            node.value->neighbours.push_back(n->value.get());
        }
    }

    /* sort by the number of neighbours */
    sort(nodes.begin(), nodes.end(), pointSort);

    /* filter the points */
    BOOST_FOREACH(NNSearch::KDNode& node, nodes) {

        BOOST_FOREACH(Point* p, node.value->neighbours) {
            // mark the node removed
            p->filtered = true;

            // remove the pointer to the parent node
            auto predicate = [&node](Point* point) { return point == node.value.get(); };
            remove_if(p->neighbours.begin(), p->neighbours.end(), predicate);
        }
    }

    /* store remaining points */
    vector<Vector3D<> > filtered;

    BOOST_FOREACH(const NNSearch::KDNode& n, nodes) {
        if (n.value->filtered == false) {
            filtered.push_back(n.value->point);
        }
    }

    DEBUG << "* points before filtering = " << points.size() << endl;
    DEBUG << "* points after filtering = " << filtered.size() << endl;

    return filtered;
}

double getAxisAlignment(const Vector3D<>& axis, const vector<Transform3D<> >& transforms, double radius) {
    vector<Vector3D<> > versors = getVersors(axis, transforms);
    if (versors.size() == 0) return 0.0;

    vector<Vector3D<> > filtered_versors = filterPoints(versors, radius);

    double axis_alignment = 1.0 - 1.0 * filtered_versors.size() / versors.size();

    return axis_alignment;
}

double getPositionAlignment(const vector<Transform3D<> >& transforms, double radius) {
    vector<Vector3D<> > points;
    BOOST_FOREACH (const Transform3D<>& t, transforms) {
        points.push_back(t.P());
    }
    
    if (points.size() == 0) return 0.0;

    vector<Vector3D<> > filtered_points = filterPoints(points, radius);

    double position_alignment = 1.0 - 1.0 * filtered_points.size() / points.size();

    return position_alignment;
}

QualityIndexValue AlignmentIndexCalculator::calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps) {
    vector<Transform3D<> > ts_after;
    typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget p, grasps->getAllTargets()) {

        // we take grasps with either success or interference
        if (
                p.second->getResult()->testStatus == GraspResult::Success
                || p.second->getResult()->testStatus == GraspResult::Interference
                ) {
            rw::math::Transform3D<> poseAfter = inverse(p.second->getResult()->objectTtcpLift);

            ts_after.push_back(poseAfter);
        }
    }
    
    DEBUG << "Calculating position alignment..." << endl;
    double p_alignment = getPositionAlignment(ts_after, _positionFilteringRadius);
    DEBUG << "Calculating X alignment..." << endl;
    double x_alignment = getAxisAlignment(Vector3D<>::x(), ts_after, _angleFilteringRadius);
    DEBUG << "Calculating Y alignment..." << endl;
    double y_alignment = getAxisAlignment(Vector3D<>::y(), ts_after, _angleFilteringRadius);
    DEBUG << "Calculating Z alignment..." << endl;
    double z_alignment = getAxisAlignment(Vector3D<>::z(), ts_after, _angleFilteringRadius);

    DEBUG << "* P alignment = " << p_alignment << endl;
    DEBUG << "* x alignment = " << x_alignment << endl;
    DEBUG << "* y alignment = " << y_alignment << endl;
    DEBUG << "* z alignment = " << z_alignment << endl;

    double alignment = (p_alignment + x_alignment + y_alignment + z_alignment) / 4.0;

    DEBUG << "* total alignment = " << alignment << endl;

    return alignment;
}
