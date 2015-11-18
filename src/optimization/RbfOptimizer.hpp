/* 
 * File:   RbfOptimizer.hpp
 * Author: dagothar
 *
 * Created on November 17, 2015, 10:28 PM
 */

#pragma once

#include "Optimizer.hpp"
#include <Python.h>

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

            //! Configuration.
            struct Configuration {
                unsigned maxNOfIterations;
                unsigned maxNOfEvaluations;

                Configuration() : maxNOfIterations(100), maxNOfEvaluations(1000) {
                }
            };

        public:
            RbfOptimizer(const Configuration& configuration = Configuration());

            virtual ~RbfOptimizer();

            virtual math::Vector minimize(math::ObjectiveFunction::Ptr function, const math::Vector& initialGuess);

            Configuration getConfiguration() const {
                return _configuration;
            }

            void setConfiguration(const Configuration& configuration) {
                _configuration = configuration;
            }

        protected:
            //! Callback used by the python RBF wrapper
            static PyObject* callback(PyObject* module, PyObject* args);

        private:
            Configuration _configuration;
        };

    }
}


