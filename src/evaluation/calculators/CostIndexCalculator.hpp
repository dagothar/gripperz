/* 
 * File:   CostIndexCalculator.hpp
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
             * @class CostIndexCalculator
             * @brief
             */
            class CostIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<CostIndexCalculator> Ptr;

            public:
                CostIndexCalculator();

                virtual ~CostIndexCalculator();
                
                virtual QualityIndex calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            private:

            };

        }
    }
}

