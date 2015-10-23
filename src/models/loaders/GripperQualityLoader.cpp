/* 
 * File:   GripperQualityLoader.cpp
 * Author: dagothar
 * 
 * Created on October 23, 2015, 1:08 PM
 */

#include "GripperQualityLoader.hpp"
#include <models/GripperQualityFactory.hpp>

using namespace std;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace boost::property_tree;

GripperQualityLoader::GripperQualityLoader() {
}

GripperQualityLoader::~GripperQualityLoader() {
}

QualityIndex readIndex(const boost::property_tree::ptree& tree) {
    QualityIndexKey key = tree.get<QualityIndexKey>("<xmlattr>.name");
    QualityIndexValue value = tree.get_value<QualityIndexValue>();

    return QualityIndex(key, value);
}

GripperQuality::Ptr GripperQualityLoader::read(const boost::property_tree::ptree& tree) {
    GripperQuality::Ptr quality = GripperQualityFactory::makeGripperQuality();

    BOOST_FOREACH(const ptree::value_type& node, tree) {
        if (node.first == "index") {
            QualityIndex i = readIndex(node.second);
            quality->setIndex(i.first, i.second);
        }
    }

    return quality;
}

pair<string, ptree> GripperQualityLoader::write(GripperQuality::Ptr object) {
    ptree tree;

    BOOST_FOREACH(const QualityIndex& p, object->getIndices()) {
        ptree node;
        node.put("<xmlattr>.name", p.first);
        node.put_value(p.second);
        tree.add_child("index", node);
    }

    return make_pair("quality", tree);
}
