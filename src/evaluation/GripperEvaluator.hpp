/* 
 * File:   GripperEvaluator.hpp
 * Author: dagothar
 *
 * Created on October 13, 2015, 12:54 PM
 */

#pragma once

#include "QualityIndexCalculator.hpp"

namespace gripperz {
    namespace evaluation {

        /**
         * @class GripperEvaluator
         * @brief Queries a set of calulators to obtain gripper quality
         */
        class GripperEvaluator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperEvaluator> Ptr;

        public:
            GripperEvaluator();

            virtual ~GripperEvaluator();
            
            virtual models::GripperQuality evaluate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);
            
            void addQualityIndexCalculator(QualityIndexCalculator::Ptr calculator);

        private:
            std::vector<QualityIndexCalculator::Ptr> _calculators;
        };

    }
}


