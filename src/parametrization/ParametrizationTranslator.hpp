/* 
 * File:   ParametrizationTranslator.hpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 10:24 AM
 */

#pragma once

#include <rw/common/Ptr.hpp>

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
        public:
            //! Constructor.
            ParametrizationTranslator();

            //! Destructor.
            virtual ~ParametrizationTranslator();
        };
    }
}
