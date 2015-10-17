/* 
 * File:   AlignmentIndexCalculator.hpp
 * Author: dagothar
 *
 * Created on October 13, 2015, 11:32 AM
 */

#pragma once

#include "../QualityIndexCalculator.hpp"

namespace gripperz {
    namespace evaluation {
        namespace calculators {

            /**
             * @class AlignmentIndexCalculator
             * @brief
             */
            class AlignmentIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<AlignmentIndexCalculator> Ptr;

            public:
                AlignmentIndexCalculator();

                virtual ~AlignmentIndexCalculator();

                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

            private:

            };

        }
    }
}