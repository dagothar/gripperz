/**
 * @file VersorAlignment.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-19
 */

#include "VersorAlignment.hpp"
#include <rwlibs/algorithms/PointModel.hpp>
#include <rw/math.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
using namespace std;
using namespace rw::math;
using namespace rwlibs::task;
using namespace rwlibs::algorithms;


VersorAlignment::VersorAlignment(const Configuration& conf) :
	_conf(conf)
{
}


VersorAlignment::~VersorAlignment() {
}


double calculatePoseVariance(vector<Transform3D<> >& ts_before, vector<Transform3D<> >& ts_after, const vector<long unsigned>& inliers) {
	if (ts_before.size() != ts_after.size()) {
		RW_THROW ("Poses vectors size mismatch!");
	}
	
	if (inliers.size() > ts_after.size()) {
		RW_THROW ("Inliers size bigger than pose vector size!");
	}
	
	double mean = 0.0;
	double variance = 0.0;
	
	Rotation3DAngleMetric<double> metric;
	
	/* calculate mean */
	vector<double> diffs;
	BOOST_FOREACH (long unsigned idx, inliers) {
		double diff = metric.distance(ts_before[idx].R(), ts_after[idx].R());
		diffs.push_back(diff);
		mean += diff;
	}
	mean /= inliers.size();
	
	DEBUG << "Model mean = " << mean << endl;
	
	/* calculate variance */
	BOOST_FOREACH (double diff, diffs) {
		double var = diff - mean;
		variance += var * var;
	}
	double deviation = sqrt(variance) / inliers.size();
	variance /= inliers.size();
	
	DEBUG << "Model variance = " << variance << endl;
	DEBUG << "Model deviation = " << deviation << endl;
	
	return variance;
}


double VersorAlignment::calculateAlignment(GraspTask::Ptr tasks) {
	/* extract pose data before & after*/
	vector<Transform3D<> > ts_before, ts_after;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	
	int n = 0;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		// we take grasps with either success or interference
		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::Interference
		) {
			rw::math::Transform3D<> poseBefore = inverse(p.second->getResult()->objectTtcpApproach);
			rw::math::Transform3D<> poseAfter = inverse(p.second->getResult()->objectTtcpLift);
			
			ts_before.push_back(poseBefore);
			ts_after.push_back(poseAfter);
			
			++n;
		}
	}
	
	/* extract versors from poses after */
	vector<Vector3D<> > vx, vy, vz;
	BOOST_FOREACH (const Transform3D<>& t, ts_after) {
		vx.push_back(t.R() * Vector3D<>::x());
		vy.push_back(t.R() * Vector3D<>::y());
		vz.push_back(t.R() * Vector3D<>::z());
	}
	
	/* find point models in versors */
	vector<PointModel> xmodels = PointModel::findModels(vx, _conf.iterations, _conf.minInliers, _conf.dataThreshold, _conf.modelThreshold);
	vector<PointModel> ymodels = PointModel::findModels(vy, _conf.iterations, _conf.minInliers, _conf.dataThreshold, _conf.modelThreshold);
	vector<PointModel> zmodels = PointModel::findModels(vz, _conf.iterations, _conf.minInliers, _conf.dataThreshold, _conf.modelThreshold);
	
	DEBUG << "Found " << xmodels.size() << " x models." << endl;
	DEBUG << "Found " << ymodels.size() << " y models." << endl;
	DEBUG << "Found " << zmodels.size() << " z models." << endl;
	
	vector<PointModel> models;
	models.insert(models.end(), xmodels.begin(), xmodels.end());
	models.insert(models.end(), ymodels.begin(), ymodels.end());
	models.insert(models.end(), zmodels.begin(), zmodels.end());
	
	double alignment = 0.01; // offset so it's not always 0
	
	if (models.size() > 0) {
			
		/* pick best model (in terms of inliers */
		sort(models.begin(), models.end());
		reverse(models.begin(), models.end());
		
		PointModel& bestModel = models.front();
		
		DEBUG << " BEST: " << bestModel << ", QUALITY: " << bestModel.getQuality() << ", INLIERS: " << bestModel.getNumberOfInliers() << endl;
		
		int totalInliers = 0;
		BOOST_FOREACH (const PointModel& m, models)
		{
			//PointModel& m = bestModel;
			vector<long unsigned> inliers = m.getInlierIndices();
			int nInliers = inliers.size();
			totalInliers += nInliers;
			alignment += calculatePoseVariance(ts_before, ts_after, inliers) * nInliers;
		}
		
		alignment /= totalInliers;
	}
	
	DEBUG << "Alignment = " << alignment << endl;
	
	return alignment;
}
