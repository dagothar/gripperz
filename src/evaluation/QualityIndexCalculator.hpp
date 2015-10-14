/* 
 * File:   QualityIndexCalculator.hpp
 * Author: dagothar
 *
 * Created on October 13, 2015, 11:10 AM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <models/OldGripper.hpp>
#include <grasps/Types.hpp>

namespace gripperz {
    namespace evaluation {

        /**
         * @class QualityIndexCalculator
         * @brief Interface for quality index calculators
         */
        class QualityIndexCalculator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<QualityIndexCalculator> Ptr;

        public:
            QualityIndexCalculator();

            virtual ~QualityIndexCalculator();

            virtual models::GripperQuality::QualityIndexValue calculate(models::OldGripper::Ptr gripper, grasps::Grasps grasps) = 0;

        private:

        };

    }
}


