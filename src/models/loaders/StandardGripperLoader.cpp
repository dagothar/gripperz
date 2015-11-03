/* 
 * File:   StandardGripperLoader.cpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 2:49 PM
 */

#include "StandardGripperLoader.hpp"
#include "models/ParametrizedGeometryGripper.hpp"
#include "models/StandardGripper.hpp"

using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace rw::common;
using namespace std;
using namespace boost::property_tree;

StandardGripperLoader::StandardGripperLoader() {
}

StandardGripperLoader::~StandardGripperLoader() {
}

Gripper::Ptr StandardGripperLoader::read(const boost::property_tree::ptree& tree) {
    ParametrizedGeometryGripper::Ptr base_gripper = ParametrizedGeometryGripperLoader::read(tree).cast<ParametrizedGeometryGripper>();
    ParametrizedGeometryGripper::Ptr gripper = ownedPtr(new StandardGripper(*base_gripper));

    return gripper;
}

std::pair<std::string, boost::property_tree::ptree> StandardGripperLoader::write(Gripper::Ptr object) {
    StandardGripper::Ptr gripper = object.cast<StandardGripper>();

    pair<string, ptree> trunk = ParametrizedGeometryGripperLoader::write(gripper.cast<ParametrizedGeometryGripper>());
    ptree& tree = trunk.second;

    tree.put("<xmlattr>.class", "StandardGripper");

    return make_pair(trunk.first, trunk.second);
}
