/* 
 * File:   RbfOptimizer.hpp
 * Author: dagothar
 *
 * Created on November 17, 2015, 10:28 PM
 */

#pragma once

#include "Optimizer.hpp"

namespace gripperz {
    namespace optimization {

        /**
         * @class RbfOptimizer
         * @brief
         */
        class RbfOptimizer : public Optimizer {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<RbfOptimizer> Ptr;

        public:
            RbfOptimizer();

            virtual ~RbfOptimizer();

            virtual math::Vector minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);
        };

    }
}


