/**
 * @file TaskGenerator.hpp
 * @brief Based on ParJawGripSampler by Jimmy
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

# pragma once

#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <rw/math.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <vector>
#include <string>
#include <rw/geometry/TriMeshSurfaceSampler.hpp>

#include <context/TaskDescription.hpp>
#include "SurfaceSample.hpp"

namespace gripperz {
namespace grasps {

/**
 * @class TaskGenerator
 * @brief Provides interface for basic task generator.
 */
class TaskGenerator {
public:
	/// Smart pointer type to this class.
	typedef rw::common::Ptr<TaskGenerator> Ptr;

public:
	TaskGenerator(context::TaskDescription::Ptr td, const std::vector<SurfaceSample>& ssamples=std::vector<SurfaceSample>());
	
	/**
	 * @brief Generates a number of tasks
	 * .
	 * Needs a collision detector which typically is generated automatically for a given workcell by RobWorkStudio and
	 * is aware of given workcell. In addition to grasp targets, this method also generates the allSamples vector, which consists of targets
	 * that only passed the parallel normals check, and not the collision check. This vector can be accessed by
	 * getAllSamples() method, and is used for calculating the coverage.
	 *
	 * A vector of pre-sampled surface samples can be provided, so the same sample set can be used to test different grippers.
	 * If the nSamples is provided (>0), the target generation continues until nSamples of samples were used.
	 *
	 * @param nTargets [in] number of targets to generate
	 * @param state [in] state
	 */
	virtual rwlibs::task::GraspTask::Ptr generateTask(
		int nTargets,
		rw::kinematics::State state,
		std::vector<SurfaceSample>* ssamples = NULL,
		int nSamples = 0
	);
	
	//! Temporary.
	virtual rwlibs::task::GraspTask::Ptr generateTasks(
		int nTargets,
		rw::kinematics::State state
	);

	/// Get previously generated tasks.
	rwlibs::task::GraspTask::Ptr getTasks() {
		return _tasks;
	}

	/// Get all samples made during task generation.
	rwlibs::task::GraspTask::Ptr getSamples() {
		return _samples;
	}
	
	std::vector<SurfaceSample>& getSurfaceSamples() { return _ssamples; }
	void setSurfaceSamples(const std::vector<SurfaceSample>& ssamples) { _ssamples = ssamples; }

	/**
	 * @brief Filters grasp tasks.
	 *
	 * For each task, removes other tasks in its neighbourhood which is defined as a 6D box (including both
	 * position and orientation). The number of remaining grasps may be then used for coverage calculation.
	 * Filtering is only applied to tasks with Success status, and removed grasps have status changed to
	 * Timeout. The original task is returned.
	 *
	 * @todo Make the method return copied task vector with filtered out targets actually removed.
	 *
	 * @param distance [in] dimension of 6D box used for finding neighbouring grasps
	 */
	static rwlibs::task::GraspTask::Ptr filterTasks(
			const rwlibs::task::GraspTask::Ptr tasks,
			rw::math::Q diff = rw::math::Q(7, 0.01, 0.01, 0.01, 0.1, 0.1, 0.1, 15 * rw::math::Deg2Rad)
	);

	/**
	 * @brief Counts tasks with specified status.
	 */
	static int countTasks(
		const rwlibs::task::GraspTask::Ptr tasks,
		const rwlibs::task::GraspResult::TestStatus status
	);

	/**
	 * @brief Makes a copy of a task
	 *
	 * @param onlySuccesses [in] only copy over succesful tasks
	 *
	 * Tricky.
	 */
	static rwlibs::task::GraspTask::Ptr copyTasks(
			const rwlibs::task::GraspTask::Ptr tasks,
			bool onlySuccesses = false
	);

	/**
	 * @brief Creates a new task with perturbed targets
	 *
	 * @param tasks [inout] target of operation
	 * @param sigma_p [in] std deviation for position change
	 * @param sigma_a [in] std deviation for the angle change
	 * @param perturbations [in] number of perturbed grasps to generate
	 */
	static rwlibs::task::GraspTask::Ptr addPerturbations(
		rwlibs::task::GraspTask::Ptr tasks,
		double sigma_p,
		double sigma_a,
		int perturbations
	);

	/**
	 * @brief Generates samples on the surface.
	 */
	SurfaceSample sample(
		const rw::kinematics::State& state,
		rw::geometry::TriMeshSurfaceSampler& sampler,
		rw::proximity::ProximityModel::Ptr object,
		rw::proximity::ProximityModel::Ptr ray,
		rw::proximity::CollisionStrategy::Ptr cstrategy
	);

protected:
	/**
	 * @brief Helper function for moving gripper TCP frame into position.
	 */
	static void moveFrameW(
		const rw::math::Transform3D<>& wTtcp,
		rw::kinematics::Frame* tcp,
		rw::kinematics::MovableFrame* base,
		rw::kinematics::State& state
	);

	// data
	context::TaskDescription::Ptr _td;

	rw::math::Q _openQ;
	rw::math::Q _closeQ;

	rwlibs::task::GraspTask::Ptr _tasks;
	rwlibs::task::GraspTask::Ptr _samples;
	
	std::vector<SurfaceSample> _ssamples;
};

} /* grasps */
} /* gripperz */
