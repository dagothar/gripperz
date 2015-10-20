/* 
 * File:   Parametrized.hpp
 * Author: dagothar
 *
 * Created on October 20, 2015, 10:30 PM
 */

#pragma once

#include "Parametrization.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class Parametrized
         * @brief Interface for parametrized objects
         */
        class Parametrized {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Parametrized> Ptr;

        public:
            Parametrized(Parametrization::Ptr parametrization);

            virtual ~Parametrized();

            void setParametrization(Parametrization::Ptr _parametrization) {
                this->_parametrization = _parametrization;
            }

            Parametrization::Ptr getParametrization() const {
                return _parametrization;
            }

        private:
            Parametrization::Ptr _parametrization;

        };

    }
}


