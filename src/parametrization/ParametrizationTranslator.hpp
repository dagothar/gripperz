/* 
 * File:   ParametrizationTranslator.hpp
 * Author: dagothar
 *
 * Created on October 21, 2015, 9:38 AM
 */

#pragma once

#include "Parametrization.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class ParametrizationTranslator
         * @brief Converts between Parametrization and numerical vectors
         */
        class ParametrizationTranslator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizationTranslator> Ptr;

            typedef std::vector<ParameterValue> ValueVector;

        public:

            /**
             * Constructor.
             * @param names
             */
            ParametrizationTranslator(const Parametrization::ParameterNameList& names = Parametrization::ParameterNameList());
            
            virtual ~ParametrizationTranslator();
            
            /**
             * Converts parametrization to a vector.
             * @param parametrization
             * @return 
             */
            virtual ValueVector parametrizationToVector(Parametrization::Ptr parametrization);

            /**
             * Creates a new parametrization based on a vector of values
             * @param vector
             * @return 
             */
            virtual Parametrization::Ptr vectorToParametrization(const ValueVector& vector);
            
            /**
             * Applies update to an existing parametrization based on the vector values
             * @param parametrization
             * @param vector
             * @return
             */
            virtual Parametrization::Ptr applyVectorToParametrization(Parametrization::Ptr parametrization, const ValueVector& vector);

            void setParameterNameList(const Parametrization::ParameterNameList& _parameterNameList) {
                this->_parameterNameList = _parameterNameList;
            }

            Parametrization::ParameterNameList getParameterNameList() const {
                return _parameterNameList;
            }
            
        private:
            Parametrization::ParameterNameList _parameterNameList;
        };

    }
}


