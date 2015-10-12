/* 
 * File:   Gripper.hpp
 * Author: dagothar
 *
 * Created on October 12, 2015, 2:20 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>

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

            void SetName(const std::string& name) {
                this->_name = name;
            }

            std::string GetName() const {
                return _name;
            }

        private:
            std::string _name;
        };

    }
}


