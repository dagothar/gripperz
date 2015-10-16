/* 
 * File:   Gripper.hpp
 * Author: dagothar
 *
 * Created on October 12, 2015, 2:20 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "GripperQuality.hpp"

namespace gripperz {
    namespace models {

        /**
         * @class Gripper
         * @brief
         */
        class Gripper {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Gripper> Ptr;

        public:
            Gripper(const std::string& name);

            virtual ~Gripper();

            void setName(const std::string& name) {
                this->_name = name;
            }

            std::string getName() const {
                return _name;
            }

            void setQuality(GripperQuality::Ptr quality) {
                this->_quality = quality;
            }

            GripperQuality::Ptr getQuality() const {
                return _quality;
            }
            
            /**
             * Calculates the cost of the gripper design
             * @return the cost of the design (0 - 1)
             */
            //virtual double getCost() = 0;
            
            /**
             * Calculates a physical feasibility of the design
             * @return a physical feasibility of the design (0 - 1)
             */
            //virtual double getFeasibility() = 0;

        private:
            std::string _name;
            GripperQuality::Ptr _quality;
        };

    }
}


