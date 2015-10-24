/**
 * @file AlignmentSimulator.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "BasicSimulator.hpp"

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
                    double limit = 0.01,
                    unsigned nThreads = 1,
                    AlignmentMetric::Ptr metric = rw::math::MetricFactory::makeTransform3DMetric<double>(1.0, 1.0)
                    );

            virtual ~AlignmentSimulator();

            double getAlignmentLimit() const {
                return _alignmentLimit;
            }

            void setAlignmentLimit(double limit) {
                _alignmentLimit = limit;
            }

            AlignmentMetric::Ptr getMetric() {
                return _metric;
            }

            void setMetric(AlignmentMetric::Ptr metric) {
                _metric = metric;
            }

        protected:
            /**
             * @brief Evaluates a grasp and changes the status if necessary.
             * Changes status to Interference, if interference limit violated.
             */
            virtual void evaluateGrasp(SimState& sstate);

        private:
            double _alignmentLimit;
            AlignmentMetric::Ptr _metric;
        };

    } /* simulation */
} /* gripperz */
