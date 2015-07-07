#include "GripperTaskSimulator.hpp"

#include <rw/math/MetricFactory.hpp>
#include <rwlibs/algorithms/StablePose1DModel.hpp>
#include <rwlibs/algorithms/StablePose0DModel.hpp>
#include <algorithm>
#include <vector>
#include <grasps/TaskGenerator.hpp>


#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
using namespace rwsim;
using namespace rwsim::simulation;
using namespace rw::common;
using namespace rwlibs::algorithms;
using namespace gripperz::simulation;
using namespace gripperz::context;
using namespace gripperz::models;
using namespace gripperz::grasps;


GripperTaskSimulator::GripperTaskSimulator(
	Gripper::Ptr gripper,
	rwlibs::task::GraspTask::Ptr tasks,
	rwlibs::task::GraspTask::Ptr samples,
	TaskDescription::Ptr td,
	int nThreads
) :
	GraspTaskSimulator(td->getDynamicWorkCell(), nThreads),
	_gripper(gripper),
	_td(td),
	_samples(samples)
{
	setWallTimeLimit(10.0);
	setSimTimeLimit(0.0);
	load(tasks);
}

void GripperTaskSimulator::graspFinished(SimState& sstate) {
	/* After each grasp is finished, its quality should be asessed.
	 * First, grasp has to pass interference measurement. When the sum
	 * of individual object interferences exceed limit specified in task description,
	 * grasp status is changed from Success to Interference.
	 * Then, if wrench space measurement result is below specified minimum wrench,
	 * grasp status is changed from Success to ObjectDropped.
	 */
	if (!_td) {
		RW_THROW("NULL task description!");
	}

	if ((sstate._target->getResult()->testStatus == GraspResult::Success
			|| sstate._target->getResult()->testStatus
					== GraspResult::ObjectSlipped)
			&& calculateInterference(sstate, _td->getInitState())
					> _td->getInterferenceLimit()) {

		DEBUG << "Grasp above interference limit." << endl;
		sstate._target->getResult()->testStatus = GraspResult::Interference;
	}

	if ((sstate._target->getResult()->testStatus == GraspResult::Success
			|| sstate._target->getResult()->testStatus
					== GraspResult::ObjectSlipped)
			&& calculateWrench(sstate) < _td->getWrenchLimit()) {

		DEBUG << "Grasp below wrench limit." << endl;
		sstate._target->getResult()->testStatus = GraspResult::ObjectDropped;
	}

	printGraspResult(sstate);
}

double GripperTaskSimulator::calculateInterference(SimState& sstate,
		const rw::kinematics::State& state0) {
	//return 0.0;

	const State& state1 = sstate._state; //getSimulators()[0]->getState();

	double interference = 0.0;
	vector<double> interferenceDistances;
	vector<double> interferenceAngles;
	vector<double> interferences;
	
	BOOST_FOREACH (Object::Ptr obj, _td->getInterferenceObjects()) {

		Transform3D<> Tbefore = obj->getBase()->getTransform(state0);
		Transform3D<> Tafter = obj->getBase()->getTransform(state1);

		double d = MetricUtil::dist2(Tafter.P(), Tbefore.P());
		interferenceDistances.push_back(d);

		double a = angle(Tbefore * Vector3D<>::z(), Tafter * Vector3D<>::z());
		interferenceAngles.push_back(a);

		Metric<Transform3D<> >::Ptr metric =
				MetricFactory::makeTransform3DMetric<double>(1.0, 1.0);
		double objInt = metric->distance(Tbefore, Tafter);

		interference += objInt;
		interferences.push_back(objInt);
	}

	GraspResult::Ptr result = sstate._target->getResult();

	result->interferenceDistances = interferenceDistances;
	result->interferenceAngles = interferenceAngles;
	result->interferences = interferences;
	result->interference = interference;

	return interference;
}

double GripperTaskSimulator::calculateWrench(SimState& sstate) const {
	
	Q& qual = sstate._target->getResult()->qualityAfterLifting;

	if (qual.size() >= 1) {
		return qual(1);
	} else {
		return 0.0;
	}
}

double GripperTaskSimulator::calculateCoverage(double actualRatio) {
	if (!_gtask || !_samples) {
		RW_WARN("NULL tasks or samples");
		return 0.0;
	}


	double coverage = 0.0;

	Q covDist = _td->getCoverageDistance();
	double R = 2.0 * sin(0.25 * covDist(1));
	Q diff(7, covDist(0), covDist(0), covDist(0), R, R, R, covDist(2));

	/* okTargets is the number of succesful targets after filtering +
	 * the number of slippages + the number of interferences */
	GraspTask::Ptr coverageTasks = TaskGenerator::filterTasks(_gtask, diff);
	int okTargets = TaskGenerator::countTasks(coverageTasks,
			GraspResult::Success);
	okTargets += TaskGenerator::countTasks(coverageTasks,
			GraspResult::ObjectSlipped);
	okTargets += TaskGenerator::countTasks(coverageTasks,
			GraspResult::Interference);

	int allTargets = TaskGenerator::countTasks(
			TaskGenerator::filterTasks(_samples, diff),
			GraspResult::UnInitialized);

	DEBUG << "Requested targets: " << getNrTargets() << " / Samples: "
			<< _samples->getAllTargets().size() << endl;
	DEBUG << "Targets (S+I) after filtering: " << okTargets
			<< " / Samples after filtering: " << allTargets << endl;
	coverage = 1.0 * okTargets / (allTargets * actualRatio);

	return coverage;
}

bool sortf(double a, double b) {
	return (a > b);
}

rw::math::Q GripperTaskSimulator::calculateWrenchMeasurement() const {
	/* this only takes into account succesful grasps */
	vector<double> wrenches; // used to find the top 10%
	Q wrench(3, 0, 0, 0);

	int successes = 0;
	typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

	BOOST_FOREACH (TaskTarget p, _gtask->getAllTargets()) {

		if (p.second->getResult()->testStatus == GraspResult::Success
				|| p.second->getResult()->testStatus
						== GraspResult::ObjectSlipped) {
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
	if (successes == 0)
		successes = 1;
	wrench(0) /= successes;
	wrench(1) /= successes;
	wrench(2) /= num;

	return wrench;
}

template<class T>
double getPoseAlignment(vector<Rotation3D<> >& rot_before,
		vector<Rotation3D<> >& rot_after, int successes,
		TaskDescription::AlignmentModelParameters params) {
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



double GripperTaskSimulator::calculateAlignment() const {
	double alignment = 0.0;

	// store rotations
	int successes = 0;
	vector<Rotation3D<> > rot_before, rot_after;
	typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, _gtask->getAllTargets()) {

		// we only take succesful grasps
		if (p.second->getResult()->testStatus == GraspResult::Success) {
			rw::math::Transform3D<> poseApproach = inverse(
					p.second->getResult()->objectTtcpApproach);
			rw::math::Transform3D<> poseLift = inverse(
					p.second->getResult()->objectTtcpLift);

			rot_before.push_back(poseApproach.R());
			rot_after.push_back(poseLift.R());

			++successes;
		}
	}

	// use RANSAC to find the most likely stable pose
	DEBUG << "Trying to find 0D stable poses..." << endl;
	double alignment0 = getPoseAlignment<StablePose0DModel>(rot_before,
			rot_after, successes, _td->getAlignmentModelParameters(0));
	DEBUG << "Trying to find 1D stable poses..." << endl;
	double alignment1 = getPoseAlignment<StablePose1DModel>(rot_before,
			rot_after, successes, _td->getAlignmentModelParameters(1));

	alignment = _td->getAlignmentModelParameters(0).weight * alignment0
			+ _td->getAlignmentModelParameters(1).weight * alignment1;

	DEBUG << "Alignment= " << alignment << endl;

	return 1.0 * alignment; // scaling factor
}

double GripperTaskSimulator::calculateShape() {
	return 0.0;
}

double GripperTaskSimulator::calculateQuality() {
	return 0.0;
}

void GripperTaskSimulator::printGraspResult(SimState& sstate) {
	INFO << "Grasp result " << getNrTargetsDone() << ": "
			<< GraspTask::toString(
					(GraspTask::TestStatus) sstate._target->getResult()->testStatus)
			<< endl;

	DEBUG << " I: " << sstate._target->getResult()->interference;
	DEBUG << " W: " << sstate._target->getResult()->qualityAfterLifting << endl;

	if ((GraspResult::TestStatus) sstate._target->getResult()->testStatus
			== GraspResult::Success) {
		rw::math::Transform3D<> poseApproach = inverse(
				sstate._target->getResult()->objectTtcpApproach);
		DEBUG << " Pose after approach: " << poseApproach.P() << " "
				<< RPY<>(poseApproach.R()) << endl;

		rw::math::Transform3D<> poseLift = inverse(
				sstate._target->getResult()->objectTtcpLift);
		DEBUG << " Pose after lift: " << poseLift.P() << " "
				<< RPY<>(poseLift.R()) << endl;
	}
}

void GripperTaskSimulator::simulationFinished(SimState& sstate) {
	evaluateGripper();
}

void GripperTaskSimulator::evaluateGripper() {
	TaskDescription::Qualities& w = _td->getWeights();

	DEBUG << "EVALUATION - " << endl;
	DEBUG << _gripper->getName() << " - Evaluating..." << endl;

	int successes = TaskGenerator::countTasks(_gtask, GraspResult::Success);
	int interferences = TaskGenerator::countTasks(_gtask,
			GraspResult::Interference);
	int slippages = TaskGenerator::countTasks(_gtask, GraspResult::ObjectSlipped);
	int drops = TaskGenerator::countTasks(_gtask, GraspResult::ObjectDropped);
	int failures = TaskGenerator::countTasks(_gtask,
			GraspResult::SimulationFailure);
	int samples = TaskGenerator::countTasks(_samples, GraspResult::UnInitialized);
	int removed = TaskGenerator::countTasks(_gtask, GraspResult::Filtered);
	int filtered = getNrTargets() - removed;
	int actual = filtered - failures;

	successes += slippages;

	DEBUG << "CALCULATING SUCCESS RATIO - " << endl;
	DEBUG << " * Targets generated: " << getNrTargets() << endl;
	DEBUG << " * After preliminary filtering: " << filtered << endl;
	DEBUG << " * Actual simulated targets (without sim failure): " << actual
			<< endl;

	DEBUG << "* Outcomes (success/interference/drop/fail): " << successes << "/"
			<< interferences << "/" << drops << "/" << failures << endl;

	double successRatio = (1.0 * successes / actual);

	/* alignment */
	DEBUG << "CALCULATING ALIGNMENT - " << endl;
	double alignment = calculateAlignment();
	DEBUG << " * Alignment: " << alignment << endl;

	/* wrench */
	DEBUG << "CALCULATING WRENCH - " << endl;
	Q wrenchMeasurement = calculateWrenchMeasurement();
	double wrench = wrenchMeasurement(1);
	double topwrench = wrenchMeasurement(2);

	/* coverage */
	DEBUG << "CALCULATING COVERAGE - " << endl;
	double coverage = calculateCoverage(1.0 * actual / filtered);

	/* Calculate quality
	 * 
	 * quality is based on success, but with subtracted penalty for max stress:
	 * penalty = stress / stresslimit clamped to [0, 1]
	 * quality = success - penalty clamped to [0, 1]
	 */
	DEBUG << "CALCULATING STRESS - " << endl;
	double maxstress = _gripper->getMaxStress();
	double penaltyS = maxstress / _td->getStressLimit();

	DEBUG << " * Max stress: " << maxstress << endl;
	DEBUG << " * Penalty (stress): " << penaltyS << endl;

	DEBUG << "CALCULATING VOLUME - " << endl;
	double volume = _gripper->getVolume();
	double penaltyV = volume;

	DEBUG << " * Volume: " << volume << endl;
	DEBUG << " * Penalty (volume): " << penaltyV << endl;

	DEBUG << "CALCULATING PENALTY - " << endl;
	double penalty = w.stress * penaltyS + w.volume * penaltyV;

	DEBUG << " * Penalty (volume+stress): " << penalty << endl;

	DEBUG << "CALCULATING QUALITY - " << endl;
	DEBUG << " * Weights: " << endl;
	DEBUG << "  - Wsuccess= " << w.success << endl;
	DEBUG << "  - Wcoverage= " << w.coverage << endl;
	DEBUG << "  - Wwrench= " << w.wrench << endl;
	DEBUG << "  - Wtopwrench= " << w.topwrench << endl;
	DEBUG << "  - Wstress= " << w.stress << endl;
	DEBUG << "  - Wvolume= " << w.volume << endl;
	DEBUG << "  - Walignment= " << w.alignment << endl;

	double quality = w.success * successRatio + w.coverage * coverage
			+ w.wrench * wrench + w.topwrench * topwrench
			+ w.alignment * alignment - penalty;
	if (quality < 0.0)
		quality = 0.0;

	// save data to gripper result

	_quality.nOfExperiments = getNrTargets();
	_quality.nOfSuccesses = successes;
	_quality.nOfSamples = samples;
	_quality.coverage = coverage;
	_quality.success = successRatio;
	_quality.wrench = wrench;
	_quality.topwrench = topwrench;
	_quality.maxstress = maxstress;
	_quality.volume = volume;
	_quality.alignment = alignment;
	_quality.quality = quality;

	DEBUG << "DONE - " << endl;
	DEBUG << _quality << endl;
}