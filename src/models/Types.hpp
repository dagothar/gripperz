/* 
 * File:   Types.hpp
 * Author: dagothar
 *
 * Created on October 14, 2015, 4:49 PM
 */

#pragma once

#include <string>

namespace gripperz {
    namespace models {

        //! Defines the type for the quality index key
        typedef std::string QualityIndexKey;

        //! Defines the type for the quality index value
        typedef double QualityIndexValue;

        //! Defines the quality index type
        typedef std::pair<QualityIndexKey, QualityIndexValue> QualityIndex;

    }
}


