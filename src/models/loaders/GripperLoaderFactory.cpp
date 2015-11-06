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
#include "models/ParametrizedGeometryGripper.hpp"
#include "ParametrizedGeometryGripperLoader.hpp"
#include "StandardGripperLoader.hpp"
#include "models/StandardGripper.hpp"
#include "models/BasicParametrizedGripper.hpp"

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
    } else if (cls == "ParametrizedGeometryGripper") {
        return ownedPtr(new ParametrizedGeometryGripperLoader());
    } else if (cls == "StandardGripper") {
        return ownedPtr(new StandardGripperLoader());
    }

    return ownedPtr(new BaseGripperLoader());
}

GripperLoader::Ptr GripperLoaderFactory::getLoader(Gripper::Ptr grp) {
    if (dynamic_cast<ParametrizedGripper*>(grp.get())) {
        return getLoader("ParametrizedGripper");
    } else if (dynamic_cast<BasicParametrizedGripper*>(grp.get())) {
        return getLoader("BasicParametrizedGripper");
    } else if (dynamic_cast<ParametrizedGeometryGripper*>(grp.get())) {
        return getLoader("ParametrizedGeometryGripper");
    } else if (dynamic_cast<StandardGripper*>(grp.get())) {
        return getLoader("StandardGripper");
    } 
    
    return getLoader("");
}