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
                /**
                 * Constructor.
                 * @param stressLimit [in] max. stress (in MPa)
                 */
                StressIndexCalculator(double stressLimit = 25);

                virtual ~StressIndexCalculator();

                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

                                /**
                 * Set max. stress.
                 * @param _stressLimit [in] in MPa
                 */
                void setStressLimit(double _stressLimit) {
                    this->_stressLimit = _stressLimit;
                }

                double getStressLimit() const {
                    return _stressLimit;
                }

            private:
                double _stressLimit;

            };

        }
    }
}

