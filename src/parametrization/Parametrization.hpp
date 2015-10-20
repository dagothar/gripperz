/* 
 * File:   Parametrization.hpp
 * Author: dagothar
 *
 * Created on October 20, 2015, 10:32 PM
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <vector>
#include <map>
#include <string>
#include "Types.hpp"

namespace gripperz {
    namespace parametrization {

        /**
         * @class Parametrization
         * @brief
         */
        class Parametrization {
        public:
            //! Smart pointer.
            typedef rw::common::Ptr<Parametrization> Ptr;
            
            typedef std::vector<Parameter> ParameterList;
            
            typedef std::vector<ParameterName> ParameterNameList;

        public:
            Parametrization();

            virtual ~Parametrization();
            
            void addParameter(const Parameter& p) {
                _parameters.insert(p);
            }

        private:
            std::map<ParameterName, ParameterValue> _parameters;
        };

    }
}


