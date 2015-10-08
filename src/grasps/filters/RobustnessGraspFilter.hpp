/* 
 * File:   RobustnessGraspFilter.hpp
 * Author: dagothar
 *
 * Created on October 8, 2015, 3:27 PM
 */

#pragma once

#include "GraspFilter.hpp"

namespace gripperz {
    namespace grasps {
        namespace filters {

            /**
             * @class RobustnessGraspFilter
             * @brief Used for calculating robustness index.
             */
            class RobustnessGraspFilter : public GraspFilter {
            public:
                //! Smart pointer.
                typedef rw::common::Ptr<RobustnessGraspFilter> Ptr;

            public:
                RobustnessGraspFilter(unsigned nOfGrasps, double sigmaPos, double sigmaAngle);

                virtual ~RobustnessGraspFilter();

                virtual Grasps filter(Grasps grasps);

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

                void setNumberOfGrasps(unsigned _numberOfGrasps) {
                    this->_numberOfGrasps = _numberOfGrasps;
                }

                unsigned getNumberOfGrasps() const {
                    return _numberOfGrasps;
                }

            private:
                unsigned _numberOfGrasps;
                double _positionUncertainty;
                double _angleUncertainty;
            };

        }
    }
}

