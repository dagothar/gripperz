/* 
 * File:   Parametrization.cpp
 * Author: dagothar
 * 
 * Created on October 20, 2015, 10:32 PM
 */

#include "Parametrization.hpp"
#include <boost/foreach.hpp>
#include <rw/common/macros.hpp>

using namespace std;
using namespace gripperz::parametrization;

Parametrization::Parametrization() {
}

Parametrization::Parametrization(const ParameterList& list) {
    BOOST_FOREACH(const Parameter& p, list) {
        addParameter(p);
    }
}

Parametrization::Parametrization(const ParameterNameList& list) {
    BOOST_FOREACH(const ParameterName& p, list) {
        addParameter(p);
    }
}

Parametrization::~Parametrization() {
}

void Parametrization::addParameter(const Parameter& p) {
    typedef map<ParameterName, ParameterValue>::iterator ParameterIterator;
    pair<ParameterIterator, bool> insertion_result = _parameters.insert(p);
    
    if (!insertion_result.second) {
        RW_WARN("Parameter not added (key value exists already)!"); // TODO: should throw?
    }
}

void Parametrization::addParameter(const ParameterName& name, const ParameterValue& value) {
    addParameter(make_pair(name, value));
}

void Parametrization::addParameter(const ParameterName& name) {
    addParameter(make_pair(name, 0.0));
}

ParameterValue Parametrization::getParameter(const ParameterName& name) const {
    return _parameters.at(name);
}

Parametrization::ParameterList Parametrization::getParameterList() const {
    ParameterList list;
    
    BOOST_FOREACH(const Parameter& p, _parameters) {
        list.push_back(p);
    }
    
    return list;
}

Parametrization::ParameterNameList Parametrization::getParameterNameList() const {
    ParameterNameList list;
    
    BOOST_FOREACH (const Parameter& p, _parameters) {
        list.push_back(p.first);
    }
    
    return list;
}

bool Parametrization::hasParameter(const ParameterName& name) const {
    if (_parameters.find(name) == _parameters.end()) {
        return false;
    } else {
        return true;
    }
}

void Parametrization::setParameter(const Parameter& p) {
    _parameters.at(p.first) = p.second;
}

void Parametrization::setParameter(const ParameterName& name, const ParameterValue& value) {
    setParameter({name, value});
}