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
                CostIndexCalculator(double volumeLimit = 100.0);

                virtual ~CostIndexCalculator();

                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

                void setVolumeLimit(double _volumeLimit) {
                    this->_volumeLimit = _volumeLimit;
                }

                double getVolumeLimit() const {
                    return _volumeLimit;
                }

            private:
                double _volumeLimit;
            };

        }
    }
}

