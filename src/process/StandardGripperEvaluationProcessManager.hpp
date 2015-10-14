/**
 * @file StandardEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#pragma once

#include "GripperEvaluationProcessManager.hpp"

namespace gripperz {
    namespace process {

        /**
         * @class StandardEvaluationManager
         * @brief Uses standard evaluation (in the same process).
         */
        class StandardGripperEvaluationProcessManager : public GripperEvaluationProcessManager {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<StandardGripperEvaluationProcessManager> Ptr;

        public:
            StandardGripperEvaluationProcessManager(
                    context::TaskDescription::Ptr context,
                    grasps::GraspSource::Ptr graspSource,
                    simulation::GripperSimulator::Ptr simulator,
                    evaluation::OldGripperEvaluator::Ptr evaluator,
                    const Configuration& configuration
                    );

            virtual ~StandardGripperEvaluationProcessManager();

            context::TaskDescription::Ptr getContext() {
                return _context;
            }

            grasps::GraspSource::Ptr getGraspSource() {
                return _graspSource;
            }

            void setGraspSource(grasps::GraspSource::Ptr source) {
                _graspSource = source;
            }

            simulation::GripperSimulator::Ptr getSimulator() {
                return _simulator;
            }

            void setSimulator(simulation::GripperSimulator::Ptr sim) {
                _simulator = sim;
            }

            evaluation::OldGripperEvaluator::Ptr getEvaluator() {
                return _evaluator;
            }

            void setEvaluator(evaluation::OldGripperEvaluator::Ptr evaluator) {
                _evaluator = evaluator;
            }

            virtual models::GripperQuality::Ptr evaluateGripper(models::OldGripper::Ptr gripper);

        protected:
            virtual void applyGripperParametrization(models::OldGripper::Ptr gripper, rw::kinematics::State& state);

        private:
            context::TaskDescription::Ptr _context;
            grasps::GraspSource::Ptr _graspSource;
            simulation::GripperSimulator::Ptr _simulator;
            evaluation::OldGripperEvaluator::Ptr _evaluator;

        };

    } /* evaluation */
} /* gripperz */