/**
 * @file extract_poses.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-16
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <rw/rw.hpp>
#include <rw/RobWork.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <rwlibs/algorithms/kdtree/KDTreeQ.hpp>


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
using namespace rwlibs::task;


string toString(const Transform3D<>& t) {
	stringstream sstr;
	
	Vector3D<> pos = t.P();
	Rotation3D<> rot = t.R();
	RPY<> rpy(rot);
	
	sstr << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << rpy[0] << ", " << rpy[1] << ", " << rpy[2];
	
	return sstr.str();
}


int main(int argc, char* argv[]) {
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Debug);
	
	/*
	 * Read CLI.
	 */
	if (argc < 2) {
		cout << "Please provide task filename" << endl;
	}
	string filename = argv[1];
	
	/*
	 * Load tasks.
	 */
	cout << "Loading tasks from: " << filename << endl;
	GraspTask::Ptr tasks = GraspTask::load(filename);
	
	/*
	 * For all succesful targets, extract poses
	 */
	ofstream before("before.csv");
	ofstream after("after.csv");
	vector<Rotation3D<>> rot_before, rot_after;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		/* we take grasps with either success or interference */
		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::Interference
		) {
			rw::math::Transform3D<> poseApproach = inverse(p.second->getResult()->objectTtcpApproach);
			rw::math::Transform3D<> poseLift = inverse(p.second->getResult()->objectTtcpLift);
			
			rot_before.push_back(poseApproach.R());
			rot_after.push_back(poseLift.R());

			cout << "BEFORE: " << toString(poseApproach) << endl;
			before << toString(poseApproach) << endl;
			
			cout << "AFTER: " << toString(poseLift) << endl;
			after << toString(poseLift) << endl;
		}
	}
	before.close();
	after.close();
	
	/* find pose with the highest number of neighbours */
	typedef int ValueType; // index
	typedef KDTreeQ<ValueType> NNSearch;
	vector<NNSearch::KDNode> nodes;
	
	int idx = 0;
	BOOST_FOREACH (Rotation3D<> p, rot_after) {
		
		Q key(4);

		EAA<> eaa(p);
		key[0] = eaa.axis()(0);
		key[1] = eaa.axis()(1);
		key[2] = eaa.axis()(2);
		key[3] = eaa.angle();
		
		nodes.push_back(NNSearch::KDNode(key, idx));
		
		++idx;
	}
	
	NNSearch *nntree = NNSearch::buildTree(nodes);
    std::list<const NNSearch::KDNode*> result;
    
    Q diff(4, 0.01, 0.01, 0.01, 0.1);
    
    int maxR = 0;
    BOOST_FOREACH (NNSearch::KDNode& node, nodes) {

		result.clear();
		Q key = node.key;
		nntree->nnSearchRect(key-diff, key+diff, result);

		cout << "Pose " << node.value << ": " << result.size() << endl;
		
		if (result.size() > maxR) maxR = result.size();
	}
	
	cout << "MAX= " << maxR << endl;
	
	return 0;
}
