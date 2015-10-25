/* 
 * File:   GripperLoaderFactory.cpp
 * Author: dagothar
 * 
 * Created on October 23, 2015, 11:35 PM
 */

#include "GripperLoaderFactory.hpp"
#include "BaseGripperLoader.hpp"
#include "ParametrizedGripperLoader.hpp"
#include "BasicParametrizedGripperLoader.hpp"

using namespace rw::common;
using namespace gripperz::models;
using namespace gripperz::models::loaders;

GripperLoaderFactory::GripperLoaderFactory() {
}

GripperLoaderFactory::~GripperLoaderFactory() {
}

GripperLoader::Ptr GripperLoaderFactory::getLoader(const std::string& cls) {
    if (cls == "ParametrizedGripper") {
            return ownedPtr(new ParametrizedGripperLoader());
    } else if (cls == "BasicParametrizedGripper") {
        return ownedPtr(new BasicParametrizedGripperLoader());
    }
            
    return ownedPtr(new BaseGripperLoader());
}