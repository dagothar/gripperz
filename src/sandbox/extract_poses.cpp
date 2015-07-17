/**
 * @file extract_poses.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-16
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <rw/rw.hpp>
#include <rw/RobWork.hpp>
#include <rwlibs/task/GraspTask.hpp>


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


string toStringEAA(const Transform3D<>& t) {
	stringstream sstr;
	
	Vector3D<> pos = t.P();
	Rotation3D<> rot = t.R();
	EAA<> eaa(rot);
	
	sstr << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << eaa.axis()(0) << ", " << eaa.axis()(1) << ", " << eaa.axis()(2) << ", " << eaa.angle();
	
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
	 * For all succesful targets, extract position and rpys.
	 */
	ofstream before("before.csv");
	ofstream after("after.csv");
	ofstream eaabefore("eaa_before.csv");
	ofstream eaaafter("eaa_after.csv");
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		/* we take grasps with either success or interference */
		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::Interference
		) {
			rw::math::Transform3D<> poseApproach = inverse(p.second->getResult()->objectTtcpApproach);
			rw::math::Transform3D<> poseLift = inverse(p.second->getResult()->objectTtcpLift);

			cout << "BEFORE: " << toString(poseApproach) << " EAA: " << toStringEAA(poseApproach) << endl;
			before << toString(poseApproach) << endl;
			eaabefore << toStringEAA(poseApproach) << endl;
			
			cout << "AFTER: " << toString(poseLift) << " EAA: " << toStringEAA(poseLift) << endl;
			after << toString(poseLift) << endl;
			eaaafter << toStringEAA(poseLift) << endl;
		}
	}
	before.close();
	after.close();
	eaabefore.close();
	eaaafter.close();
	
	return 0;
}
