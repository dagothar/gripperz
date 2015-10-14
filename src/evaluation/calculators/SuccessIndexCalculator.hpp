/* 
 * File:   SuccessIndexCalculator.hpp
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
             * @class SuccessIndexCalculator
             * @brief
             */
            class SuccessIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<SuccessIndexCalculator> Ptr;

            public:
                SuccessIndexCalculator();

                virtual ~SuccessIndexCalculator();
                
                virtual models::GripperQuality::QualityIndexValue calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            private:

            };

        }
    }
}

