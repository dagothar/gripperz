/**
 * @file ClusteringAlignment.cpp
 * @author Adam Wolniakowski
 * @date 2015-08-15
 */

#include "ClusteringAlignment.hpp"
#include <rwlibs/algorithms/kdtree/KDTree.hpp>
#include <rwlibs/algorithms/kdtree/KDTreeQ.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
using namespace rwlibs::algorithms;
using namespace rwlibs::task;
using namespace rw::common;
using namespace rw::math;
using namespace std;


ClusteringAlignment::ClusteringAlignment(const Configuration& config) :
	_conf(config)
{
}


ClusteringAlignment::~ClusteringAlignment() {
}


vector<Vector3D<> > getVersors(const Vector3D<>& axis, const vector<Transform3D<> > transforms) {
	vector<Vector3D<> > versors;
	
	BOOST_FOREACH (const Transform3D<>& t, transforms) {
		Vector3D<> v = t.R() * axis;
		versors.push_back(v);
	}
	
	return versors;
}


struct Point {
	typedef rw::common::Ptr<Point> Ptr;
	
	Vector3D<> point;
	bool filtered;
	
	Point(const Vector3D<> v) :
		point(v),
		filtered(false)
	{}
};


vector<Vector3D<> > filterPoints(const vector<Vector3D<> >& points, double radius=0.1) {
	/* build search tree */
	typedef Point::Ptr ValueType;
	typedef KDTreeQ<ValueType> NNSearch;
	vector<NNSearch::KDNode> nodes;
	
	BOOST_FOREACH (const Vector3D<>& p, points) {
		Q key(3);
		key[0] = p[0];
		key[1] = p[1];
		key[2] = p[2];
		
		nodes.push_back(NNSearch::KDNode(key, ownedPtr(new Point(p))));
	}
	NNSearch *nntree = NNSearch::buildTree(nodes);
    
	/* filter points */
    std::list<const NNSearch::KDNode*> result;
    Q diff(3, radius, radius, radius);
    
    BOOST_FOREACH (NNSearch::KDNode& node, nodes) {
		if (node.value->filtered != true) {
			result.clear();
			Q key = node.key;
			nntree->nnSearchRect(key-diff, key+diff, result);

			BOOST_FOREACH (const NNSearch::KDNode* n, result) {
				if (n->key == node.key) continue;
				
				const_cast<NNSearch::KDNode*>(n)->value->filtered = true;
			}
		}
	}
	
	/* store remaining points */
	vector<Vector3D<> > filtered;
	
	BOOST_FOREACH (const NNSearch::KDNode& n, nodes) {
		if (n.value->filtered == false) {
			filtered.push_back(n.value->point);
		}
	}
	
	DEBUG << "* points before filtering = " << points.size() << endl;
	DEBUG << "* points after filtering = " << filtered.size() << endl;
	
	return filtered;
}


double getAxisAlignment(const Vector3D<>& axis, const vector<Transform3D<> > transforms, double radius) {
	vector<Vector3D<> > versors = getVersors(axis, transforms);
	if (versors.size() == 0) return 0.0;
	
	vector<Vector3D<> > filtered_versors = filterPoints(versors, radius);
	
	double axis_alignment = 1.0 - 1.0 * filtered_versors.size() / versors.size();
	
	return axis_alignment;
}


double ClusteringAlignment::adjustRadius(double radius, double ntargets) {
	double adjusted_radius = radius; // / sqrt(ntargets / 100.0);
	
	return adjusted_radius;
}


double ClusteringAlignment::calculateAlignment(rwlibs::task::GraspTask::Ptr tasks) {
	vector<Transform3D<> > ts_after;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		// we take grasps with either success or interference
		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::Interference
		) {
			rw::math::Transform3D<> poseAfter = inverse(p.second->getResult()->objectTtcpLift);
			
			ts_after.push_back(poseAfter);
		}
	}
	
	double filtering_radius = adjustRadius(_conf.filteringRadius, ts_after.size());
	DEBUG << "Adjusted radius = " << filtering_radius << endl;
	
	DEBUG << "Calculating X alignment..." << endl;
	double x_alignment = getAxisAlignment(Vector3D<>::x(), ts_after, filtering_radius);
	DEBUG << "Calculating Y alignment..." << endl;
	double y_alignment = getAxisAlignment(Vector3D<>::y(), ts_after, filtering_radius);
	DEBUG << "Calculating Z alignment..." << endl;
	double z_alignment = getAxisAlignment(Vector3D<>::z(), ts_after, filtering_radius);
	
	DEBUG << "* x alignment = " << x_alignment << endl;
	DEBUG << "* y alignment = " << y_alignment << endl;
	DEBUG << "* z alignment = " << z_alignment << endl;
	
	double alignment = (x_alignment + y_alignment + z_alignment) / 3.0;
	
	DEBUG << "* total alignment = " << alignment << endl;
	
	return alignment;
}
