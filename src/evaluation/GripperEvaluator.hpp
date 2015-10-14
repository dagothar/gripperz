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
         * @brief Queries a set of calculators to obtain gripper quality
         */
        class GripperEvaluator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<GripperEvaluator> Ptr;

            //! Type for the internal (key, calculator) pair storage
            typedef std::pair<models::QualityIndexKey, QualityIndexCalculator::Ptr> KeyCalculator;

        public:
            GripperEvaluator();

            virtual ~GripperEvaluator();

            virtual models::GripperQuality::Ptr evaluate(models::OldGripper::Ptr gripper, grasps::Grasps grasps);

            void addQualityIndexCalculator(const models::QualityIndexKey& indexName, QualityIndexCalculator::Ptr calculator);
            
            std::vector<models::QualityIndexKey> getIndexKeys() const;

        private:
            std::vector<KeyCalculator> _calculators;
        };

    }
}


