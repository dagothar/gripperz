/* 
 * File:   ParametrizationTranslator.hpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 10:24 AM
 */

#pragma once

#include "Parametrization.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class ParametrizationTranslator
         * @brief Facilitates translation between Parametrization and numeric vectors
         */
        class ParametrizationTranslator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizationTranslator> Ptr;

            typedef std::vector<ParameterValue> ValueVector;
            
        public:
            //! Constructor.
            ParametrizationTranslator();

            //! Destructor.
            virtual ~ParametrizationTranslator();

            /**
             * Converts parametrization to a vector.
             * @param parametrization
             * @return 
             */
            virtual ValueVector parametrizationToVector(Parametrization::Ptr parametrization) = 0;

            /**
             * Creates a new parametrization based on a vector of values
             * @param vector
             * @return 
             */
            virtual Parametrization::Ptr vectorToParametrization(const ValueVector& vector) = 0;

            /**
             * Applies update to an existing parametrization based on the vector values
             * @param parametrization
             * @param vector
             * @return
             */
            virtual Parametrization::Ptr applyVectorToParametrization(Parametrization::Ptr parametrization, const ValueVector& vector) = 0;

        };
    }
}
