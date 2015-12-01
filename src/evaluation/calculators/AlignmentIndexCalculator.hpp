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
                AlignmentIndexCalculator(double positionFilteringRadius = 0.0001, double angleFilteringRadius = 0.001);

                virtual ~AlignmentIndexCalculator();

                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

                void setPositionFilteringRadius(double _filteringRadius) {
                    this->_positionFilteringRadius = _filteringRadius;
                }

                double getPositionFilteringRadius() const {
                    return _positionFilteringRadius;
                }

                void setAngleFilteringRadius(double _angleFilteringRadius) {
                    this->_angleFilteringRadius = _angleFilteringRadius;
                }

                double getAngleFilteringRadius() const {
                    return _angleFilteringRadius;
                }

            private:
                double _positionFilteringRadius;
                double _angleFilteringRadius;
            };

        }
    }
}