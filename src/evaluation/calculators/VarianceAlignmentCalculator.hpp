/* 
 * File:   VarianceAlignmentCalculator.hpp
 * Author: dagothar
 *
 * Created on December 4, 2015, 4:14 PM
 */

#pragma once

#include "../QualityIndexCalculator.hpp"

namespace gripperz {
    namespace evaluation {
        namespace calculators {

            /**
             * @class VarianceAlignmentCalculator
             * @brief
             */
            class VarianceAlignmentCalculator : public QualityIndexCalculator {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<VarianceAlignmentCalculator> Ptr;

            public:
                VarianceAlignmentCalculator(double sigma_pos, double sigma_angle);

                virtual ~VarianceAlignmentCalculator();
                
                virtual models::QualityIndexValue calculate(models::Gripper::Ptr gripper, grasps::Grasps grasps);

                void setAngleUncertainty(double _angleUncertainty) {
                    this->_angleUncertainty = _angleUncertainty;
                }

                double getAngleUncertainty() const {
                    return _angleUncertainty;
                }

                void setPositionUncertainty(double _positionUncertainty) {
                    this->_positionUncertainty = _positionUncertainty;
                }

                double getPositionUncertainty() const {
                    return _positionUncertainty;
                }
                
            private:
                double _positionUncertainty;
                double _angleUncertainty;

            };

        }
    }

}