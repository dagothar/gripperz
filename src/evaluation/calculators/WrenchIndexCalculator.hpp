/* 
 * File:   WrenchIndexCalculator.hpp
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
             * @class WrenchIndexCalculator
             * @brief
             */
            class WrenchIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<WrenchIndexCalculator> Ptr;

            public:
                WrenchIndexCalculator();

                virtual ~WrenchIndexCalculator();
                
                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

            private:

            };

        }
    }
}

