/**
 * @file testOptimization.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <models/GripperBuilder.hpp>
#include <process/GripperEvaluationProcessManager.hpp>
#include <evaluation/GripperQualityExtractor.hpp>
#include <math/MultiObjectiveFunction.hpp>
#include <boost/function.hpp>

namespace gripperz {
    namespace evaluation {

        /**
         * @class GripperObjectiveFunction
         * @brief
         */
        class GripperObjectiveFunction : public math::MultiObjectiveFunction {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperObjectiveFunction> Ptr;
            
        public:
            GripperObjectiveFunction(
                    models::GripperBuilder::Ptr builder,
                    process::GripperEvaluationProcessManager::Ptr manager,
                    GripperQualityExtractor::Ptr extractor
                    );

            virtual ~GripperObjectiveFunction();

            virtual std::vector<double> evaluate(const std::vector<double>& x);

            models::GripperBuilder::Ptr getBuilder() {
                return _builder;
            }

            void setBuilder(models::GripperBuilder::Ptr builder) {
                _builder = builder;
            }

            process::GripperEvaluationProcessManager::Ptr getEvaluationManager() {
                return _manager;
            }

            void setEvaluationManager(process::GripperEvaluationProcessManager::Ptr manager) {
                _manager = manager;
            }

            GripperQualityExtractor::Ptr getQualityExtractor() {
                return _extractor;
            }

            void setQualityExtractor(GripperQualityExtractor::Ptr extractor) {
                _extractor = extractor;
            }

            //! Returns last evaluated gripper.

            models::Gripper::Ptr getLastGripper() const {
                return _lastGripper;
            }

            //! Returns last obtained gripper quality.

            models::GripperQuality::Ptr getLastGripperQuality() const {
                return _lastQuality;
            }

            //! Sets function evaluation callback.
            void setCallback(const boost::function<void(models::Gripper::Ptr)>& cb) {
                _callback = cb;
            }

        private:
            models::GripperBuilder::Ptr _builder;
            process::GripperEvaluationProcessManager::Ptr _manager;
            GripperQualityExtractor::Ptr _extractor;

            models::Gripper::Ptr _lastGripper;
            models::GripperQuality::Ptr _lastQuality;

            boost::function<void(models::Gripper::Ptr) > _callback;
        };

    } /* evaluation */
} /* gripperz */
