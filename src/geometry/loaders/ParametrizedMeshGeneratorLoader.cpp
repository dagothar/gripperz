/* 
 * File:   ParametrizedMeshGeneratorLoader.cpp
 * Author: dagothar
 *
 * Created on November 3, 2015, 1:02 PM
 */

#include "ParametrizedMeshGeneratorLoader.hpp"
#include "geometry/OldFingerGenerator.hpp"
#include "geometry/OpenSCADGenerator.hpp"
#include "parametrization/loaders/ParametrizationLoader.hpp"

using namespace gripperz::geometry;
using namespace gripperz::geometry::loaders;
using namespace gripperz::parametrization;
using namespace gripperz::parametrization::loaders;
using namespace rw::common;
using namespace std;
using namespace boost::property_tree;

ParametrizedMeshGeneratorLoader::ParametrizedMeshGeneratorLoader() {
}

ParametrizedMeshGeneratorLoader::~ParametrizedMeshGeneratorLoader() {
}

ParametrizedMeshGenerator::Ptr ParametrizedMeshGeneratorLoader::read(const boost::property_tree::ptree& tree) {
    string cls = tree.get<string>("<xmlattr>.class");

    ParametrizedMeshGenerator::Ptr gen = NULL;
    if (cls == "OldFingerGenerator") {
        gen = ownedPtr(new OldFingerGenerator());
    } else if (cls == "OpenSCADGenerator") {
        string script = tree.get<string>("template");
        gen = ownedPtr(new OpenSCADGenerator(script));
    } else {
        RW_THROW("Unknown ParametrizedMeshGenerator class!");
        return NULL;
    }

    boost::optional<const ptree&> p_node = tree.get_child_optional("parametrization");
    if (p_node) {
        ParametrizationLoader::Ptr param_loader = new ParametrizationLoader();
        Parametrization::Ptr param = param_loader->read(*p_node);
        gen->setParametrization(param);
    } else {
        gen->setParametrization(ownedPtr(new Parametrization()));
    }

    return gen;
}

std::pair<std::string, boost::property_tree::ptree> ParametrizedMeshGeneratorLoader::write(ParametrizedMeshGenerator::Ptr object) {
    ptree tree;

    if (dynamic_cast<OldFingerGenerator*> (object.get())) {
        tree.put("<xmlattr>.class", "OldFingerGenerator");
    } else if (dynamic_cast<OpenSCADGenerator*> (object.get())) {
        tree.put("<xmlattr>.class", "OpenSCADGenerator");
        tree.put("template", dynamic_cast<OpenSCADGenerator*> (object.get())->getScriptFilename());
    } else {
        RW_THROW("Unknown ParametrizedMeshGenerator class!");
    }

    ParametrizationLoader::Ptr param_loader = new ParametrizationLoader();
    auto param_node = param_loader->write(object->getParametrization());

    tree.add_child("parametrization", param_node.second);

    return make_pair("meshGenerator", tree);
}
