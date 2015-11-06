/* 
 * File:   MasterGripperLoader.cpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 1:57 PM
 */

#include "MasterGripperLoader.hpp"
#include "GripperLoaderFactory.hpp"

using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace std;

MasterGripperLoader::MasterGripperLoader() {
}

MasterGripperLoader::~MasterGripperLoader() {
}

Gripper::Ptr MasterGripperLoader::read(const boost::property_tree::ptree& tree) {
    GripperLoaderFactory::Ptr factory = new GripperLoaderFactory();

    string cls = tree.get<string>("<xmlattr>.class");
    GripperLoader::Ptr loader = factory->getLoader(cls);

    return loader->read(tree);
}

std::pair<std::string, boost::property_tree::ptree> MasterGripperLoader::write(Gripper::Ptr object) {
    GripperLoaderFactory::Ptr factory = new GripperLoaderFactory();

    GripperLoader::Ptr loader = factory->getLoader(object);

    return loader->write(object);
}
