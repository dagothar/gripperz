/**
 * @file Context.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include <rw/common/Ptr.hpp>

namespace gripperz {
    namespace context {

        /**
         * @class Context
         * @brief Base class for storing task context.
         */
        class Context {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Context> Ptr;

        public:
            Context();

            virtual ~Context();
        };

    } /* context */
} /* gripperz */
