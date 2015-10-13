/* 
 * File:   Types.hpp
 * Author: dagothar
 *
 * Created on October 13, 2015, 9:48 AM
 */

#pragma once

#include <map>
#include <string>
#include <iostream>

namespace gripperz {
    namespace models {
        
        /**
         * Defines a type for the gripper quality
         */
        typedef std::map<std::string, double> GripperQuality;
        
        std::ostream& operator<<(std::ostream& stream, const GripperQuality& quality);
    }
}


