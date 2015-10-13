/* 
 * File:   Types.hpp
 * Author: dagothar
 *
 * Created on October 13, 2015, 9:48 AM
 */

#ifndef MODELS_TYPES_HPP
#define	MODELS_TYPES_HPP

#include <map>
#include <string>

namespace gripperz {
    namespace models {
        
        /**
         * Defines a type for the gripper quality
         */
        typedef std::map<std::string, double> GripperQuality;
    }
}

#endif	/* TYPES_HPP */

