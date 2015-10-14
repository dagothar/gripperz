/* 
 * File:   StressIndexCalculator.hpp
 * Author: dagothar
 *
 * Created on October 13, 2015, 11:25 AM
 */

#pragma once

#include "../QualityIndexCalculator.hpp"

namespace gripperz {
    namespace evaluation {
        namespace calculators {

            /**
             * @class StressIndexCalculator
             * @brief
             */
            class StressIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<StressIndexCalculator> Ptr;

            public:
                StressIndexCalculator();

                virtual ~StressIndexCalculator();
                
                virtual models::QualityIndexValue calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            private:

            };

        }
    }
}

