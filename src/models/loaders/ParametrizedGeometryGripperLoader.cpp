/* 
 * File:   ParametrizedGeometryGripperLoader.cpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 12:44 PM
 */

#include "ParametrizedGeometryGripperLoader.hpp"

using namespace std;
using namespace rw::common;
using namespace gripperz::parametrization;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace boost::property_tree;

ParametrizedGeometryGripperLoader::ParametrizedGeometryGripperLoader() {
}

ParametrizedGeometryGripperLoader::~ParametrizedGeometryGripperLoader() {
}

Gripper::Ptr ParametrizedGeometryGripperLoader::read(const boost::property_tree::ptree& tree) {

}

std::pair<std::string, boost::property_tree::ptree> ParametrizedGeometryGripperLoader::write(Gripper::Ptr object) {

}
