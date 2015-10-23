/* 
 * File:   ParametrizedGripperLoader.cpp
 * Author: dagothar
 * 
 * Created on October 23, 2015, 2:56 PM
 */

#include "ParametrizedGripperLoader.hpp"
#include "BaseGripperLoader.hpp"
#include <parametrization/loaders/ParametrizationLoader.hpp>

using namespace std;
using namespace rw::common;
using namespace gripperz::parametrization;
using namespace gripperz::parametrization::loaders;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace boost::property_tree;

ParametrizedGripperLoader::ParametrizedGripperLoader() {
}

ParametrizedGripperLoader::~ParametrizedGripperLoader() {
}

Gripper::Ptr ParametrizedGripperLoader::read(const boost::property_tree::ptree& tree) {
    Gripper::Ptr basic_gripper = BaseGripperLoader::read(tree);
    
    ParametrizationLoader::Ptr param_loader = new ParametrizationLoader();
    Parametrization::Ptr param = param_loader->read(tree.get_child("parametrization"));
    
    ParametrizedGripper::Ptr parametrized_gripper = ownedPtr(new ParametrizedGripper(*basic_gripper, param));
    
    return parametrized_gripper;
}

pair<string, ptree> ParametrizedGripperLoader::write(Gripper::Ptr grp) {
    ParametrizedGripper::Ptr gripper = grp.cast<ParametrizedGripper>();
    
    pair<string, ptree> trunk = BaseGripperLoader::write(gripper);
    ptree& tree = trunk.second;
    
    tree.put("<xmlattr>.name", gripper->getName());
    tree.put("<xmlattr>.class", "ParametrizedGripper");
    
    ParametrizationLoader::Ptr param_loader = new ParametrizationLoader();
    auto param_node = param_loader->write(gripper->getParametrization());
    
    tree.add_child("parametrization", param_node.second);
    
    return make_pair(trunk.first, trunk.second);
}
