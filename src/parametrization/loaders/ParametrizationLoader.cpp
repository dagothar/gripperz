/* 
 * File:   ParametrizationLoader.cpp
 * Author: dagothar
 * 
 * Created on October 22, 2015, 1:19 PM
 */

#include "ParametrizationLoader.hpp"
#include <util/XMLHelpers.hpp>

using namespace std;
using namespace rw::common;
using namespace gripperz::util;
using namespace gripperz::parametrization;
using namespace gripperz::parametrization::loaders;
using namespace boost::property_tree;

ParametrizationLoader::ParametrizationLoader() {
}

ParametrizationLoader::~ParametrizationLoader() {
}

Parametrization::Ptr ParametrizationLoader::load(const std::string& filename) {

    rw::common::Ptr<ptree> tree = XMLHelpers::parseXMLFile(filename);

    return read(*tree);
}

Parameter readParameter(ptree& tree) {
    ParameterName name = tree.get<ParameterName>("<xmlattr>.name");
    ParameterValue value = tree.get<ParameterValue>("");

    return Parameter(name, value);
}

Parametrization::Ptr readParametrization(ptree& tree) {
    Parametrization::Ptr parametrization = ownedPtr(new Parametrization());

    BOOST_FOREACH(ptree::value_type& node, tree) {
        if (node.first == "parameter") {
            Parameter p = readParameter(node.second);
            parametrization->addParameter(p);
        }
    }

    return parametrization;
}

Parametrization::Ptr ParametrizationLoader::read(const ptree& tree) {
    ptree root = tree.get_child("parametrization");

    return readParametrization(root);
}

void ParametrizationLoader::save(const std::string& filename, Parametrization::Ptr parametrization) {
    ptree tree = write(parametrization);
    
    XMLHelpers::saveXMLFile(filename, &tree);
}

ptree ParametrizationLoader::write(Parametrization::Ptr parametrization) {
    ptree tree;

    BOOST_FOREACH(const Parameter& p, parametrization->getParameterList()) {
        ptree node;
        node.put("<xmlattr>.name", p.first);
        node.put_value(p.second);
        tree.add_child("parametrization.parameter", node);
    }
    
    return tree;
}
