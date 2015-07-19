/**
 * @file StablePoseAlignment.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-17
 */

#include "StablePoseAlignment.hpp"
#include <rw/math.hpp>
#include <vector>
#include <rwlibs/algorithms/StablePose0DModel.hpp>
#include <rwlibs/algorithms/StablePose1DModel.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::evaluation;
using namespace rwlibs::task;
using namespace rwlibs::algorithms;
using namespace rw::math;
using namespace std;


StablePoseAlignment::StablePoseAlignment(const Configuration& c0, const Configuration& c1) :
	_config0(c0),
	_config1(c1)
{
}


StablePoseAlignment::~StablePoseAlignment() {
}


template<class T>
double getModelAlignment(
	vector<Rotation3D<> >& rot_before,
	vector<Rotation3D<> >& rot_after,
	const StablePoseAlignment::Configuration& config
) {
	double alignment = 0.0;

	vector<T> models = T::findModels(
		rot_after,
		config.iterations,
		config.minInliers,
		config.dataThreshold,
		config.modelThreshold
	);

	if (models.size() == 0) {
		return 0.0;
	}
	
	// sort models from best quality to worst
	sort(models.begin(), models.end());
	reverse(models.begin(), models.end());

	int total_inliers = 0;
	Rotation3DAngleMetric<double> metric;
	DEBUG << "Models found (" << models.size() << "):" << endl;
	
	BOOST_FOREACH (const T& m, models) {
		DEBUG << " - StablePose: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
		
		int n_inliers = m.getNumberOfInliers();
		total_inliers += n_inliers;
		
		if (n_inliers == 0) {
			continue;
		}

		// calculate model mean and variance
		vector<size_t> indices = m.getInlierIndices();
		
		// mean
		vector<double> diffs;
		double avg_diff = 0.0;
		BOOST_FOREACH (size_t idx, indices) {
			
			double diff = metric.distance(rot_before[idx], rot_after[idx]);
			diffs.push_back(diff);
			avg_diff += diff;
		}
		avg_diff /= n_inliers;
		DEBUG << "Average difference= " << avg_diff << endl;
		
		sort(diffs.begin(), diffs.end()); // this is so we can weigh the median more

		// variance
		double variance = 0.0;
		double total_w = 0.0;
		for (unsigned i = 0; i < diffs.size(); ++i) {
			double w = 1.0 + ((i < diffs.size() - i) ? i : diffs.size() - i);
			total_w += w;
			
			double dvar = diffs[i] - avg_diff;
			variance += dvar * dvar;
		}
		variance /= total_w;
		
		variance = sqrt(variance);
		DEBUG << "Variance= " << variance << endl;

		alignment += variance * n_inliers;
		DEBUG << "Alignment so far= " << alignment << endl;
		
		break;
	}
	
	alignment = alignment / total_inliers;

	DEBUG << "Total number of inliers= " << total_inliers << endl;
	DEBUG << "Model type alignment= " << alignment << endl;

	return alignment;
}


double StablePoseAlignment::calculateAlignment(rwlibs::task::GraspTask::Ptr tasks) {
	double alignment = 0.0;

	// store rotations
	int successes = 0;
	vector<Rotation3D<> > rot_before, rot_after;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		// we take grasps with either success or interference
		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::Interference
		) {
			Transform3D<> poseApproach = inverse(p.second->getResult()->objectTtcpApproach);
			Transform3D<> poseLift = inverse(p.second->getResult()->objectTtcpLift);

			rot_before.push_back(poseApproach.R());
			rot_after.push_back(poseLift.R());

			++successes;
		}
	}

	// use RANSAC to find the most likely stable pose
	DEBUG << "Trying to find 0D stable poses..." << endl;
	double alignment0 = getModelAlignment<StablePose0DModel>(rot_before, rot_after, _config1);
	DEBUG << "Trying to find 1D stable poses..." << endl;
	double alignment1 = getModelAlignment<StablePose1DModel>(rot_before, rot_after, _config1);

	alignment =
		_config0.weight * alignment0
		+ _config1.weight * alignment1;

	DEBUG << "Alignment index= " << alignment << endl;

	return alignment;
}

