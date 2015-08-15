/**
 * @file SimpleAlignment.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-19
 */

#include "SimpleAlignment.hpp"
#include <rwlibs/algorithms/PointModel.hpp>
#include <rw/math.hpp>

#define DEBUG rw::common::Log::infoLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
using namespace std;
using namespace rw::math;
using namespace rwlibs::task;
using namespace rwlibs::algorithms;


SimpleAlignment::SimpleAlignment(const Configuration& conf) :
	_conf(conf)
{
}


SimpleAlignment::~SimpleAlignment() {
}


double SimpleAlignment::calculateAlignment(GraspTask::Ptr tasks) {
	/* extract pose data before & after*/
	vector<Transform3D<> > ts_before, ts_after;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	
	int n = 0;
	int nAllTargets = tasks->getAllTargets().size();
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
	
	double alignment = 0.0;
	
	if (models.size() > 0) {
			
		/* pick best model (in terms of inliers */
		sort(models.begin(), models.end());
		reverse(models.begin(), models.end());
		
		double totalWeight = 0.0;
		int nModels = 0;
		BOOST_FOREACH (const PointModel& m, models)
		{
			if (nModels++ >= _conf.maxModels) {
				break;
			}
			
			DEBUG << " MODEL: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
			
			vector<long unsigned> inliers = m.getInlierIndices();
			
			int nInliers = inliers.size();
			double quality = m.getQuality();
			
			double modelAlignment = 1.0 * nInliers / nAllTargets;
			double modelWeight = -log(quality);
			if (modelWeight < 0.0) modelWeight = 0.0; // to safe-guard against positive logarithms (quality > 1)
			double modelAlignmentAdjustment = modelAlignment * modelWeight;
			
			alignment += modelAlignmentAdjustment;
			totalWeight += modelWeight;
			
			DEBUG << "modelAlignmentAdjustment = " << modelAlignment << " * " << modelWeight << " = " << modelAlignmentAdjustment << endl;
		}
		
		alignment = alignment / totalWeight;
	}
	
	DEBUG << "Alignment = " << alignment << endl;
	
	return alignment + 0.01;  // offset so it's not always 0
}
