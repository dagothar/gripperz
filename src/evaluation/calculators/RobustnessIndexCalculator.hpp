/* 
 * File:   RobustnessIndexCalculator.hpp
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
             * @class RobustnessIndexCalculator
             * @brief
             */
            class RobustnessIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<RobustnessIndexCalculator> Ptr;

            public:
                RobustnessIndexCalculator();

                virtual ~RobustnessIndexCalculator();
                
                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

            private:

            };

        }
    }
}

