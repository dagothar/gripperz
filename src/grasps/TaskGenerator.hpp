/**
 * @file TaskGenerator.hpp
 * @brief Based on ParJawGripSampler by Jimmy
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

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
            TaskGenerator(context::TaskDescription::Ptr td, const std::vector<SurfaceSample>& ssamples = std::vector<SurfaceSample>());

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

            std::vector<SurfaceSample>& getSurfaceSamples() {
                return _ssamples;
            }

            void setSurfaceSamples(const std::vector<SurfaceSample>& ssamples) {
                _ssamples = ssamples;
            }

            /**
             * @brief Counts tasks with specified status.
             */
            static int countTasks(
                    const rwlibs::task::GraspTask::Ptr tasks,
                    const rwlibs::task::GraspResult::TestStatus status
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
             * @brief Generates targets for testing robustness.
             */
            static rwlibs::task::GraspTask::Ptr generateRobustnessTasks(
                    rwlibs::task::GraspTask::Ptr tasks,
                    int n,
                    double sigma_p = 0.003,
                    double sigma_a = 8.0
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

            rwlibs::task::GraspTask::Ptr _tasks;
            rwlibs::task::GraspTask::Ptr _samples;

            std::vector<SurfaceSample> _ssamples;
        };

    } /* grasps */
} /* gripperz */
