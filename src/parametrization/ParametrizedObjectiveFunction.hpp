/* 
 * File:   ParametrizedObjectiveFunction.hpp
 * Author: dagothar
 *
 * Created on November 12, 2015, 10:56 AM
 */

#pragma once

#include <math/ObjectiveFunction.hpp>
#include "ParametrizedObjective.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class ParametrizedObjectiveFunction
         * @brief
         */
        class ParametrizedObjectiveFunction : public math::ObjectiveFunction, public ParametrizedObjective {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<ParametrizedObjectiveFunction> Ptr;
        public:
            //! Constructor.
            ParametrizedObjectiveFunction(ParametrizationTranslator::Ptr translator);

            //! Destructor.
            virtual ~ParametrizedObjectiveFunction();
            
            virtual math::Scalar evaluate(const math::Vector& x);
            
        protected:
            virtual math::Scalar doEvaluate(Parametrization::Ptr param) = 0;

        };
    }
}
