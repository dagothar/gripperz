/* 
 * File:   CoverageIndexCalculator.hpp
 * Author: dagothar
 *
 * Created on October 13, 2015, 11:29 AM
 */

#pragma once

#include "../QualityIndexCalculator.hpp"

namespace gripperz {
    namespace evaluation {
        namespace calculators {

            /**
             * @class CoverageIndexCalculator
             * @brief
             */
            class CoverageIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<CoverageIndexCalculator> Ptr;

            public:
                CoverageIndexCalculator();

                virtual ~CoverageIndexCalculator();

                virtual models::GripperQuality::QualityIndexValue calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            private:

            };

        }
    }
}

