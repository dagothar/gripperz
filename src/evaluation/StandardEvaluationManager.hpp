/**
 * @file StandardEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#pragma once

#include "GripperEvaluationManager.hpp"

namespace gripperz {
    namespace evaluation {

        /**
         * @class StandardEvaluationManager
         * @brief Uses standard evaluation (in the same process).
         */
        class StandardEvaluationManager : public GripperEvaluationManager {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<StandardEvaluationManager> Ptr;

        public:
            StandardEvaluationManager(
                    context::TaskDescription::Ptr context,
                    grasps::GraspSource::Ptr graspSource,
                    simulation::GripperSimulator::Ptr simulator,
                    evaluation::GripperEvaluator::Ptr evaluator,
                    const Configuration& configuration
                    );

            virtual ~StandardEvaluationManager();

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

            evaluation::GripperEvaluator::Ptr getEvaluator() {
                return _evaluator;
            }

            void setEvaluator(evaluation::GripperEvaluator::Ptr evaluator) {
                _evaluator = evaluator;
            }

            virtual models::OldGripperQuality::Ptr evaluateGripper(models::OldGripper::Ptr gripper);

        protected:
            virtual void applyGripperParametrization(models::OldGripper::Ptr gripper, rw::kinematics::State& state);

        private:
            context::TaskDescription::Ptr _context;
            grasps::GraspSource::Ptr _graspSource;
            simulation::GripperSimulator::Ptr _simulator;
            evaluation::GripperEvaluator::Ptr _evaluator;

        };

    } /* evaluation */
} /* gripperz */
