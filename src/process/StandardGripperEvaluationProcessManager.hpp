/**
 * @file StandardEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#pragma once

#include "GripperEvaluationProcessManager.hpp"
#include <grasps/GraspSource.hpp>
#include <context/Context.hpp>
#include <simulation/GripperSimulator.hpp>
#include <evaluation/GripperEvaluator.hpp>

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
                    context::Context::Ptr context,
                    grasps::GraspSource::Ptr graspSource,
                    simulation::GripperSimulator::Ptr simulator,
                    evaluation::GripperEvaluator::Ptr evaluator
                    );

            virtual ~StandardGripperEvaluationProcessManager();

            context::Context::Ptr getContext() {
                return _context;
            }
            
            void setContext(context::Context::Ptr context) {
                _context = context;
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

            virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper);

        protected:
            virtual void registerGripper(models::Gripper::Ptr gripper);

            virtual grasps::Grasps generateGrasps();

            virtual grasps::Grasps simulateGrasps(grasps::Grasps grasps);

            virtual models::GripperQuality::Ptr doEvaluation(models::Gripper::Ptr gripper, grasps::Grasps grasps);

        private:
            context::Context::Ptr _context;
            grasps::GraspSource::Ptr _graspSource;
            simulation::GripperSimulator::Ptr _simulator;
            evaluation::GripperEvaluator::Ptr _evaluator;

        };

    } /* evaluation */
} /* gripperz */
