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
	
	Rotation3D<> rot = t.R();
	
	Vector3D<> vx = rot * Vector3D<>::x();
	Vector3D<> vy = rot * Vector3D<>::y();
	Vector3D<> vz = rot * Vector3D<>::z();
	
	sstr <<
		vx[0] << ", " << vx[1] << ", " << vx[2] << ", " <<
		vy[0] << ", " << vy[1] << ", " << vy[2] << ", " <<
		vz[0] << ", " << vz[1] << ", " << vz[2];
	
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
	ofstream versors("versors.csv");
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		/* we take grasps with either success or interference */
		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::Interference
		) {
			//rw::math::Transform3D<> poseApproach = inverse(p.second->getResult()->objectTtcpApproach);
			rw::math::Transform3D<> poseLift = inverse(p.second->getResult()->objectTtcpLift);
			
			versors << toString(poseLift) << endl;
		}
	}
	versors.close();
	
	return 0;
}
