/* 
 * File:   Types.hpp
 * Author: dagothar
 *
 * Created on October 20, 2015, 4:49 PM
 */

#pragma once

#include <string>

namespace gripperz {
    namespace parametrization {

        //! Parameter name type
        typedef std::string ParameterName;

        //! Parameter value type
        typedef double ParameterValue;

        //! Parameter type
        typedef std::pair<ParameterName, ParameterValue> Parameter;

    }
}


