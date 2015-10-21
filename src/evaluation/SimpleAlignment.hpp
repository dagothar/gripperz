/**
 * @file SimpleAlignment.hpp
 * @author Adam Wolniakowski
 * @date 2015-08-14
 */

#pragma once

#include "AlignmentCalculator.hpp"

namespace gripperz {
    namespace evaluation {

        /**
         * @class SimpleAlignment
         * @brief Calculates alignment based on model quality and number of inliers
         */
        class SimpleAlignment : public virtual AlignmentCalculator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<SimpleAlignment> Ptr;

            //! Configuration for finding a point model.

            struct Configuration {
                int iterations;
                int minInliers;
                double dataThreshold;
                double modelThreshold;
                int maxModels;

                Configuration() :
                iterations(1000),
                minInliers(5),
                dataThreshold(0.1),
                modelThreshold(0.1),
                maxModels(5) {
                }
            };

        public:
            /**
             * @brief Constructor.
             * @param conf [in] parameters for finding point model with RANSAC
             * @param c1 [in] parameters for finding stable pose 1D model with RANSAC
             */
            SimpleAlignment(const Configuration& conf = Configuration());

            virtual ~SimpleAlignment();

            //! @copydoc AlignmentCalculator::calculateAlignment
            virtual double calculateAlignment(rwlibs::task::GraspTask::Ptr tasks);

        private:
            Configuration _conf;
        };

    } /* evaluation */
} /* gripperz */
