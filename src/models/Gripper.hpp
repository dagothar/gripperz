/* 
 * File:   Gripper.hpp
 * Author: dagothar
 *
 * Created on October 12, 2015, 2:20 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include "Types.hpp"

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

            void setQuality(const GripperQuality& _quality) {
                this->_quality = _quality;
            }

            GripperQuality getQuality() const {
                return _quality;
            }

        private:
            std::string _name;
            GripperQuality _quality;
        };

    }
}


