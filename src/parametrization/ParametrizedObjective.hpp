/* 
 * File:   ParametrizedObjective.hpp
 * Author: dagothar
 *
 * Created on November 12, 2015, 11:03 AM
 */

#pragma once

#include "ParametrizationTranslator.hpp"
#include "math/Types.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class ParametrizedObjective
         * @brief Common interface for multi- and single-objective functions using Parametrization
         */
        class ParametrizedObjective {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizedObjective> Ptr;
        public:
            //! Constructor.
            ParametrizedObjective(ParametrizationTranslator::Ptr translator);

            //! Destructor.
            virtual ~ParametrizedObjective();

            void SetTranslator(ParametrizationTranslator::Ptr _translator) {
                this->_translator = _translator;
            }

            ParametrizationTranslator::Ptr GetTranslator() const {
                return _translator;
            }
            
        protected:
            virtual Parametrization::Ptr translate(const math::Vector& vector);
            
        private:
            ParametrizationTranslator::Ptr _translator;
        };
    }
}
