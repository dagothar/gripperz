/**
 * @file GripperEvaluator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "GripperEvaluator.hpp"

#include <algorithm>
#include <grasps/TaskStatistics.hpp>
#include <grasps/TaskGenerator.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <rwlibs/task/GraspTarget.hpp>
#include <rwlibs/algorithms/StablePose1DModel.hpp>
#include <rwlibs/algorithms/StablePose0DModel.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
using namespace gripperz::evaluation;
using namespace gripperz::models;
using namespace gripperz::grasps;
using namespace gripperz::context;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;
using namespace rwlibs::algorithms;


GripperEvaluator::GripperEvaluator(TaskDescription::Ptr context) :
	_context(context)
{}


GripperEvaluator::~GripperEvaluator() 
{}


GripperQuality::Ptr GripperEvaluator::evaluateGripper(Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples, rwlibs::task::GraspTask::Ptr rtasks) {
	GripperQuality::Ptr quality = ownedPtr(new GripperQuality());
	
	quality->success = calculateSuccess(gripper, tasks, samples);
	quality->robustness = calculateRobustness(gripper, tasks, samples, rtasks);
	quality->coverage = calculateCoverage(gripper, tasks, samples);
	quality->alignment = calculateAlignment(gripper, tasks, samples);
	quality->wrench = calculateWrench(gripper, tasks, samples);
	quality->maxstress = calculateStress(gripper, tasks, samples);
	quality->volume = calculateVolume(gripper, tasks, samples);
	
	return quality;
}


double GripperEvaluator::calculateSuccess(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples) {
	DEBUG << "CALCULATING SUCCESS - " << endl;
	std::vector<std::pair<class GraspSubTask*, class GraspTarget*> > allTargets = tasks->getAllTargets();
	int nAllTargets = allTargets.size();
	
	int successes = TaskStatistics::countTasksWithResult(tasks, GraspResult::Success);
	int filtered = TaskStatistics::countTasksWithResult(tasks, GraspResult::Filtered);
	int failures = TaskStatistics::countTasksWithResult(tasks, GraspResult::SimulationFailure);
	
	DEBUG << "alltargets= " << nAllTargets << endl;
	DEBUG << "successes= " << successes << endl;
	DEBUG << "filtered= " << filtered << endl;
	DEBUG << "failures= " << failures << endl;
	
	double validTasks = nAllTargets - filtered - failures;
	if (validTasks == 0) {
		RW_WARN("No valid tasks");
		return 0.0;
	}
	
	double successIndex = 1.0 * successes /  validTasks;
	
	return successIndex;
}


double GripperEvaluator::calculateRobustness(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples, rwlibs::task::GraspTask::Ptr rtasks) {
	if (rtasks == NULL) {
		RW_WARN("Not calculating robustness.");
		return 1.0;
	}
	
	DEBUG << "CALCULATING ROBUSTNESS - " << endl;
	std::vector<std::pair<class GraspSubTask*, class GraspTarget*> > allTargets = rtasks->getAllTargets();
	int nAllTargets = allTargets.size();
	
	int successes = TaskStatistics::countTasksWithResult(rtasks, GraspResult::Success);
	int filtered = TaskStatistics::countTasksWithResult(rtasks, GraspResult::Filtered);
	int failures = TaskStatistics::countTasksWithResult(rtasks, GraspResult::SimulationFailure);
	
	DEBUG << "alltargets= " << nAllTargets << endl;
	DEBUG << "successes= " << successes << endl;
	DEBUG << "filtered= " << filtered << endl;
	DEBUG << "failures= " << failures << endl;
	
	double validTasks = nAllTargets - filtered - failures;
	if (validTasks == 0) {
		RW_WARN("No valid tasks");
		return 0.0;
	}
	
	double robustnessIndex = 1.0 * successes / validTasks;
	
	return robustnessIndex;
}


double GripperEvaluator::calculateCoverage(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples) {
	DEBUG << "CALCULATING COVERAGE - " << endl;
	double coverage = 0.0;

	Q covDist = _context->getCoverageDistance();
	double R = 2.0 * sin(0.25 * covDist(1));
	Q diff(7, covDist(0), covDist(0), covDist(0), R, R, R, covDist(2));

	/* okTargets is the number of succesful targets after filtering +
	 * + the number of interferences
	 */
	GraspTask::Ptr coverageTasks = TaskGenerator::filterTasks(tasks, diff);
	
	int okTargets = TaskStatistics::countTasksWithResult(coverageTasks, GraspResult::Success);
	okTargets += TaskStatistics::countTasksWithResult(coverageTasks, GraspResult::Interference);

	int allTargets = TaskStatistics::countTasksWithResult(TaskGenerator::filterTasks(samples, diff), GraspResult::UnInitialized);

	DEBUG << "Requested targets: " << tasks->getAllTargets().size() << " / Samples: " << samples->getAllTargets().size() << endl;
	DEBUG << "Targets (S+I) after filtering: " << okTargets	<< " / Samples after filtering: " << allTargets << endl;
	
	coverage = 1.0 * okTargets / (allTargets * 1.0);

	return coverage;
}


template<class T>
double getPoseAlignment(
	vector<Rotation3D<> >& rot_before,
	vector<Rotation3D<> >& rot_after,
	int successes,
	TaskDescription::AlignmentModelParameters params
) {
	double alignment = 0.0;

	vector<T> models = T::findModels(rot_after, params.iterations,
			params.minInliers, params.dataThreshold, params.modelThreshold);

	if (models.size() == 0)
		return 0.0;
	sort(models.begin(), models.end());
	reverse(models.begin(), models.end());

	int inliers = 0;
	Rotation3DAngleMetric<double> metric;
	DEBUG << "Models found (" << models.size() << "):" << endl;
	BOOST_FOREACH (const T& m, models) {
		DEBUG << " - StablePose: " << m << ", QUALITY: " << m.getQuality()
				<< ", INLIERS: " << m.getNumberOfInliers() << endl;
		inliers += m.getNumberOfInliers();

		// calculate model mean and variance
		int n = m.getNumberOfInliers();
		vector<size_t> indices = m.getInlierIndices();
		if (indices.size() == 0)
			continue;

		// mean
		vector<double> diffs;
		double avg_diff = 0.0;
		BOOST_FOREACH (size_t idx, indices) {
			double diff = metric.distance(rot_before[idx], rot_after[idx]);
			diffs.push_back(diff);
			avg_diff += diff;
		}
		avg_diff /= n;
		DEBUG << "Average difference= " << avg_diff << endl;

		// variance
		double variance = 0.0;
		BOOST_FOREACH (double diff, diffs) {
			double dvar = diff - avg_diff;
			variance += dvar * dvar;
		}
		variance = sqrt(variance / n);
		DEBUG << "Variance= " << variance << endl;

		alignment += variance * n / successes;
		DEBUG << "Alignment so far= " << alignment << endl;
	}

	DEBUG << "Sum of inliers= " << inliers << endl;
	DEBUG << "Alignment= " << alignment << endl;

	return alignment;
}


double GripperEvaluator::calculateAlignment(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples) {
	DEBUG << "CALCULATING ALIGNMENT - " << endl;
	double alignment = 0.0;

	// store rotations
	int successes = 0;
	vector<Rotation3D<> > rot_before, rot_after;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		// we only take succesful grasps
		if (p.second->getResult()->testStatus == GraspResult::Success) {
			rw::math::Transform3D<> poseApproach = inverse(p.second->getResult()->objectTtcpApproach);
			rw::math::Transform3D<> poseLift = inverse(p.second->getResult()->objectTtcpLift);

			rot_before.push_back(poseApproach.R());
			rot_after.push_back(poseLift.R());

			++successes;
		}
	}

	// use RANSAC to find the most likely stable pose
	DEBUG << "Trying to find 0D stable poses..." << endl;
	double alignment0 = getPoseAlignment<StablePose0DModel>(rot_before,	rot_after, successes, _context->getAlignmentModelParameters(0));
	DEBUG << "Trying to find 1D stable poses..." << endl;
	double alignment1 = getPoseAlignment<StablePose1DModel>(rot_before,	rot_after, successes, _context->getAlignmentModelParameters(1));

	alignment = _context->getAlignmentModelParameters(0).weight * alignment0
		+ _context->getAlignmentModelParameters(1).weight * alignment1;

	DEBUG << "Alignment= " << alignment << endl;

	return alignment; // scaling factor
}


bool sortf(double a, double b) {
	return (a > b);
}


double GripperEvaluator::calculateWrench(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples) {
	DEBUG << "CALCULATING WRENCH - " << endl;
	
	vector<double> wrenches; // used to find the top 10%
	Q wrench(3, 0, 0, 0);

	int successes = 0;
	typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {

		if (
			p.second->getResult()->testStatus == GraspResult::Success
			|| p.second->getResult()->testStatus == GraspResult::ObjectSlipped
		) {
			successes++;

			Q result = p.second->getResult()->qualityAfterLifting;

			wrench(0) += result(0);
			wrench(1) += result(1);

			wrenches.push_back(result(1));
		}
	}

	// find top 20%
	sort(wrenches.begin(), wrenches.end(), sortf);

	int num = 0.2 * successes < 1 ? 1 : 0.2 * successes;

	if (wrenches.size() > 0) {
		for (int i = 0; i < num; ++i) {
			wrench(2) += wrenches[i];
		}
	}

	// calculate averages
	if (successes == 0) {
		successes = 1;
	}
	
	wrench(0) /= successes;
	wrench(1) /= successes;
	wrench(2) /= num;

	return wrench(1);
}


double GripperEvaluator::calculateStress(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples) {
	DEBUG << "CALCULATING STRESS - " << endl;
	double maxstress = gripper->getMaxStress();
	DEBUG << "Gripper stress= " << maxstress << endl;
	
	double stress = 1.0 - maxstress / _context->getStressLimit();
	
	if (stress < 0.0) {
		stress = 0.0;
	}
	
	return stress;
}


double GripperEvaluator::calculateVolume(models::Gripper::Ptr gripper, rwlibs::task::GraspTask::Ptr tasks, rwlibs::task::GraspTask::Ptr samples) {
	DEBUG << "CALCULATING VOLUME - " << endl;
	double gripperVolume = gripper->getVolume();
	DEBUG << "Gripper volume= " << gripperVolume << endl;
	
	double volumeIndex = 1.0 - gripperVolume / _context->getVolumeLimit();
	
	if (volumeIndex < 0.0) {
		volumeIndex = 0.0;
	}
	
	return volumeIndex;
}
