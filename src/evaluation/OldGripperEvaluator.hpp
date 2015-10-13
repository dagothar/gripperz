/**
 * @file GripperEvaluator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <models/OldGripper.hpp>
#include <context/TaskDescription.hpp>
#include <grasps/Types.hpp>
#include "AlignmentCalculator.hpp"
#include "GripperEvaluator.hpp"

namespace gripperz {
    namespace evaluation {

        /**
         * @class GripperEvaluator
         * @brief Interface for gripper evaluation.
         */
        class OldGripperEvaluator : public GripperEvaluator {
        public:
            typedef rw::common::Ptr<OldGripperEvaluator> Ptr;

        public:
            /**
             * @brief Constructor.
             * @param context [in] task description
             */
            OldGripperEvaluator(context::TaskDescription::Ptr context, AlignmentCalculator::Ptr alignmentCalculator);

            virtual ~OldGripperEvaluator();

            context::TaskDescription::Ptr getContext() {
                return _context;
            }

            void setContext(context::TaskDescription::Ptr context) {
                _context = context;
            }

            AlignmentCalculator::Ptr getAlignmentCalculator() {
                return _alignmentCalculator;
            }

            void setAlignmentCalculator(AlignmentCalculator::Ptr calc) {
                _alignmentCalculator = calc;
            }

            /**
             * @brief Tests sanity of the gripper design.
             */
            virtual bool isSane(models::OldGripper::Ptr gripper);

            /**
             * @brief Evaluates gripper quality.
             * Uses gripper parameters and statistics in task performance to calculate the gripper quality.
             */
            virtual models::OldGripperQuality::Ptr evaluate(models::OldGripper::Ptr gripper, grasps::Grasps tasks, grasps::Grasps rtasks = NULL);


            virtual models::GripperQuality evaluate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);


        protected:
            //! Calculates the success index of the gripper.
            virtual double calculateSuccess(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            //! Calculates the robustness index of the gripper.
            virtual double calculateRobustness(models::OldGripper::Ptr gripper, grasps::Grasps grasps, grasps::Grasps rgrasps = NULL);

            //! Calculates the coverage index of the gripper.
            virtual double calculateCoverage(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            //! Calculates the alignment index of the gripper.
            virtual double calculateAlignment(grasps::Grasps grasps);

            //! Calculates the wrench index of the gripper.
            virtual double calculateWrench(models::OldGripper::Ptr gripper, grasps::Grasps grasps);
            virtual double calculateTopWrench(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            //! Calculates the stress index of the gripper.
            virtual double calculateStress(models::OldGripper::Ptr gripper);

            //! Calculates the volume index of the gripper.
            virtual double calculateVolume(models::OldGripper::Ptr gripper);

        private:
            context::TaskDescription::Ptr _context;
            AlignmentCalculator::Ptr _alignmentCalculator;
        };

    } /* evaluation */
} /* gripperz */
