/* 
 * File:   SignificanceIndexCalculator.hpp
 * Author: dagothar
 *
 * Created on November 12, 2015, 5:09 PM
 */

#pragma once

#include "../QualityIndexCalculator.hpp"

namespace gripperz {
    namespace evaluation {
        namespace calculators {

            /**
             * @class SignificanceIndexCalculator
             * @brief Calculates statistical significance of the result
             */
            class SignificanceIndexCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<SignificanceIndexCalculator> Ptr;

            public:
                SignificanceIndexCalculator();

                virtual ~SignificanceIndexCalculator();
                
                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

            };

        }
    }
}

