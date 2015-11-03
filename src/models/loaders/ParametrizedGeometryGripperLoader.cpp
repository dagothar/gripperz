/* 
 * File:   ParametrizedGeometryGripperLoader.cpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 12:44 PM
 */

#include "ParametrizedGeometryGripperLoader.hpp"
#include "geometry/loaders/ParametrizedMeshGeneratorLoader.hpp"
#include "models/ParametrizedGeometryGripper.hpp"

using namespace std;
using namespace rw::common;
using namespace gripperz::geometry;
using namespace gripperz::geometry::loaders;
using namespace gripperz::parametrization;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace boost::property_tree;

ParametrizedGeometryGripperLoader::ParametrizedGeometryGripperLoader() {
}

ParametrizedGeometryGripperLoader::~ParametrizedGeometryGripperLoader() {
}

Gripper::Ptr ParametrizedGeometryGripperLoader::read(const boost::property_tree::ptree& tree) {
    ParametrizedGripper::Ptr base_gripper = ParametrizedGripperLoader::read(tree).cast<ParametrizedGripper>();
    ParametrizedGeometryGripper::Ptr gripper = ownedPtr(new ParametrizedGeometryGripper(*base_gripper));

    ParametrizedMeshGeneratorLoader::Ptr gen_loader = new ParametrizedMeshGeneratorLoader();
    ParametrizedMeshGenerator::Ptr generator = gen_loader->read(tree.get_child("meshGenerator"));
    gripper->setMeshGenerator(generator);

    return gripper;
}

std::pair<std::string, boost::property_tree::ptree> ParametrizedGeometryGripperLoader::write(Gripper::Ptr object) {
    ParametrizedGeometryGripper::Ptr gripper = object.cast<ParametrizedGeometryGripper>();

    pair<string, ptree> trunk = ParametrizedGripperLoader::write(gripper);
    ptree& tree = trunk.second;

    tree.put("<xmlattr>.class", "ParametrizedGeometryGripper");
    
    Parametrization::Ptr gen_param = gripper->getMeshGenerator()->getParametrization();
    gripper->getMeshGenerator()->setParametrization(ownedPtr(new Parametrization()));
    
    ParametrizedMeshGeneratorLoader::Ptr gen_loader = new ParametrizedMeshGeneratorLoader();
    auto gen_node = gen_loader->write(gripper->getMeshGenerator());
    tree.add_child("meshGenerator", gen_node.second);
    gripper->getMeshGenerator()->setParametrization(gen_param);

    return make_pair(trunk.first, trunk.second);
}
