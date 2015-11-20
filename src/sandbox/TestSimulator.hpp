/* 
 * File:   TestSimulator.hpp
 * Author: dagothar
 *
 * Created on November 20, 2015, 1:43 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>

namespace gripperz {
    namespace sandbox {

        /**
         * @class TestSimulator
         * @brief
         */
        class TestSimulator {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<TestSimulator> Ptr;

        public:
            TestSimulator();

            virtual ~TestSimulator();

        private:

        };

    }
}


