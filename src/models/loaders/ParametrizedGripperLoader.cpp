/* 
 * File:   ParametrizedGripperLoader.cpp
 * Author: dagothar
 * 
 * Created on October 23, 2015, 2:56 PM
 */

#include "ParametrizedGripperLoader.hpp"
#include "GripperLoader.hpp"
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

ParametrizedGripper::Ptr ParametrizedGripperLoader::read(const boost::property_tree::ptree& tree) {
    GripperLoader::Ptr gripper_loader = new GripperLoader();
    Gripper::Ptr basic_gripper = gripper_loader->read(tree);
    
    ParametrizationLoader::Ptr param_loader = new ParametrizationLoader();
    Parametrization::Ptr param = param_loader->read(tree.get_child("gripper"));
    
    ParametrizedGripper::Ptr parametrized_gripper = ownedPtr(new ParametrizedGripper(*basic_gripper, param));
    
    return parametrized_gripper;
}

boost::property_tree::ptree ParametrizedGripperLoader::write(ParametrizedGripper::Ptr object) {
    GripperLoader::Ptr gripper_loader = new GripperLoader();
    ptree tree = gripper_loader->write(object);
    
    //ptree name_node;
    //name_node.put("<xmlattr>.class", "ParametrizedGripper");
    //tree.add_child("gripper", name_node);
    
    ParametrizationLoader::Ptr param_loader = new ParametrizationLoader();
    ptree param_node = param_loader->write(object->getParametrization());
    
    tree.add_child("gripper", param_node);
    
    return tree;
}
