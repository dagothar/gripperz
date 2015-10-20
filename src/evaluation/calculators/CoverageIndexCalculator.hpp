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
                CoverageIndexCalculator(double positionFilteringRadius = 0.001, double angleFilteringRadius = 15.0);

                virtual ~CoverageIndexCalculator();

                virtual models::QualityIndexValue calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

                void setAngleFilteringRadius(double _angleFilteringRadius) {
                    this->_angleFilteringRadius = _angleFilteringRadius;
                }

                double getAngleFilteringRadius() const {
                    return _angleFilteringRadius;
                }

                void setPositionFilteringRadius(double _positionFilteringRadius) {
                    this->_positionFilteringRadius = _positionFilteringRadius;
                }

                double getPositionFilteringRadius() const {
                    return _positionFilteringRadius;
                }
                
            private:
                double _positionFilteringRadius;
                double _angleFilteringRadius;
            };

        }
    }
}

