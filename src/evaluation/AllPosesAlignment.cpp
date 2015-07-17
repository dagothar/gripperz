/**
 * @file AllPosesAlignment.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-17
 */

#include "AllPosesAlignment.hpp"
#include <rw/math.hpp>
#include <vector>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
using namespace rwlibs::task;
using namespace rw::math;
using namespace std;


AllPosesAlignment::AllPosesAlignment() {
}


AllPosesAlignment::~AllPosesAlignment() {
}


double AllPosesAlignment::calculateAlignment(rwlibs::task::GraspTask::Ptr tasks) {
	double alignment = 0.0;
	
	// calulate pose distances before and after
	Rotation3DAngleMetric<double> metric;
	vector<double> t_diffs;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		// we take grasps with either success or interference
		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::Interference
		) {
			rw::math::Transform3D<> poseApproach = inverse(p.second->getResult()->objectTtcpApproach);
			rw::math::Transform3D<> poseLift = inverse(p.second->getResult()->objectTtcpLift);

			double diff = metric.distance(poseApproach.R(), poseLift.R());
			t_diffs.push_back(diff);
		}
	}
	
	// calculate mean pose difference
	double avg_diff = 0.0;
	BOOST_FOREACH (double diff, t_diffs) {
		avg_diff += diff;
	}
	avg_diff /= t_diffs.size();
	
	DEBUG << " - Average diff: " << avg_diff << endl;
	
	// calculate variance
	double var_diff = 0.0;
	BOOST_FOREACH (double diff, t_diffs) {
		double var = diff - avg_diff;
		var_diff += var * var;
	}
	var_diff = sqrt(var_diff) / t_diffs.size();
	
	DEBUG << " - Variance: " << var_diff << endl;
	
	alignment = avg_diff;
	
	return alignment;
}
