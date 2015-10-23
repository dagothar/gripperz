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

Parameter readParameter(const ptree& tree) {
    ParameterName name = tree.get<ParameterName>("<xmlattr>.name");
    ParameterValue value = tree.get_value<ParameterValue>();

    return Parameter(name, value);
}

Parametrization::Ptr ParametrizationLoader::read(const ptree& tree) {
    Parametrization::Ptr parametrization = ownedPtr(new Parametrization());

    BOOST_FOREACH(const ptree::value_type& node, tree) {
        if (node.first == "parameter") {
            Parameter p = readParameter(node.second);
            parametrization->addParameter(p);
        }
    }

    return parametrization;
}

pair<string, ptree> ParametrizationLoader::write(Parametrization::Ptr parametrization) {
    ptree tree;

    BOOST_FOREACH(const Parameter& p, parametrization->getParameterList()) {
        ptree node;
        node.put("<xmlattr>.name", p.first);
        node.put_value(p.second);
        tree.add_child("parameter", node);
    }
    
    return make_pair("parametrization", tree);
}
