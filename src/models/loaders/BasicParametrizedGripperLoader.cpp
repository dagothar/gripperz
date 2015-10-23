/* 
 * File:   BasicParametrizedGripperLoader.cpp
 * Author: dagothar
 * 
 * Created on October 23, 2015, 2:56 PM
 */

#include "BasicParametrizedGripperLoader.hpp"
#include "models/BasicParametrizedGripper.hpp"
#include <parametrization/loaders/ParametrizationLoader.hpp>

using namespace std;
using namespace rw::common;
using namespace gripperz::parametrization;
using namespace gripperz::parametrization::loaders;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace boost::property_tree;

BasicParametrizedGripperLoader::BasicParametrizedGripperLoader() {
}

BasicParametrizedGripperLoader::~BasicParametrizedGripperLoader() {
}

Gripper::Ptr BasicParametrizedGripperLoader::read(const boost::property_tree::ptree& tree) {
    ParametrizedGripper::Ptr base_gripper = ParametrizedGripperLoader::read(tree).cast<ParametrizedGripper>();
    
    BasicParametrizedGripper::Ptr gripper = ownedPtr(new BasicParametrizedGripper(*base_gripper));
    
    return gripper;
}

pair<string, ptree> BasicParametrizedGripperLoader::write(Gripper::Ptr grp) {
    BasicParametrizedGripper::Ptr gripper = grp.cast<BasicParametrizedGripper>();
    
    pair<string, ptree> trunk = ParametrizedGripperLoader::write(gripper);
    ptree& tree = trunk.second;

    tree.put("<xmlattr>.class", "BasicParametrizedGripper");
    
    return make_pair(trunk.first, trunk.second);
}
