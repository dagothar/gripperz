/**
 * @file AlignmentSimulator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "BasicSimulator.hpp"

#include <rw/math/Transform3D.hpp>
#include <rw/math/Metric.hpp>
#include <rw/math/MetricFactory.hpp>

namespace gripperz {
    namespace simulation {

        /**
         * @class AlignmentSimulator
         * @brief Extends the BasicSimulator with checking the target object alignment
         */
        class AlignmentSimulator : public BasicSimulator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<AlignmentSimulator> Ptr;

            //! Metric used to calculate interference.
            typedef rw::math::Metric<rw::math::Transform3D<> > AlignmentMetric;

        public:
            AlignmentSimulator(
                    rwsim::dynamics::DynamicWorkCell::Ptr dwc,
                    const rw::math::Transform3D<>& expectedPose,
                    double threshold = 0.01,
                    unsigned nThreads = 1,
                    AlignmentMetric::Ptr metric = rw::math::MetricFactory::makeTransform3DMetric<double>(1.0, 1.0)
                    );

            virtual ~AlignmentSimulator();

            double getAlignmentThreshold() const {
                return _alignmentThreshold;
            }

            void setAlignmentThreshold(double limit) {
                _alignmentThreshold = limit;
            }

            AlignmentMetric::Ptr getMetric() {
                return _metric;
            }

            void setMetric(AlignmentMetric::Ptr metric) {
                _metric = metric;
            }

            void setExpectedPose(const rw::math::Transform3D<>& _expectedPose) {
                this->_expectedPose = _expectedPose;
            }

            rw::math::Transform3D<> getExpectedPose() const {
                return _expectedPose;
            }

        protected:
            /**
             * @brief Evaluates a grasp and changes the status if necessary.
             * Changes status to Interference, if interference limit violated.
             */
            virtual void evaluateGrasp(SimState& sstate);
            
            virtual double calculateAlignmentDifference(SimState& sstate);
            
            virtual void printGraspResult(SimState& sstate);

        private:
            rw::math::Transform3D<> _expectedPose;
            double _alignmentThreshold;
            AlignmentMetric::Ptr _metric;
        };

    } /* simulation */
} /* gripperz */
