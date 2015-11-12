/* 
 * File:   ParametrizedMultiObjectiveFunction.hpp
 * Author: dagothar
 *
 * Created on November 12, 2015, 11:15 AM
 */

#pragma once

#include "ParametrizedObjective.hpp"
#include "math/MultiObjectiveFunction.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class ParametrizedMultiObjectiveFunction
         * @brief
         */
        class ParametrizedMultiObjectiveFunction : public math::MultiObjectiveFunction, public ParametrizedObjective {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizedMultiObjectiveFunction> Ptr;
        public:
            //! Constructor.
            ParametrizedMultiObjectiveFunction(ParametrizationTranslator::Ptr translator);

            //! Destructor.
            virtual ~ParametrizedMultiObjectiveFunction();
            

            virtual math::Vector evaluate(const math::Vector& x);
            
        protected:
            
            virtual math::Vector doEvaluate(Parametrization::Ptr param) = 0;
        };
    }
}
