/* 
 * File:   ParametrizationTranslator.cpp
 * Author: dagothar
 * 
 * Created on October 21, 2015, 9:40 AM
 */

#include "ParametrizationTranslator.hpp"
#include <rw/common/macros.hpp>

using namespace gripperz::parametrization;
using namespace rw::common;

ParametrizationTranslator::ParametrizationTranslator(const Parametrization::ParameterNameList& names) :
_parameterNameList(names) {

}

ParametrizationTranslator::~ParametrizationTranslator() {

}

ParametrizationTranslator::ValueVector ParametrizationTranslator::parametrizationToVector(Parametrization::Ptr parametrization) {
    unsigned n = _parameterNameList.size();
    ValueVector values(n);

    for (unsigned i = 0; i < n; ++i) {
        values[i] = parametrization->getParameter(_parameterNameList[i]);
    }

    return values;
}

Parametrization::Ptr ParametrizationTranslator::applyVectorToParametrization(Parametrization::Ptr parametrization, const ValueVector& vector) {
    RW_ASSERT(vector.size() == _parameterNameList.size());

    unsigned n = _parameterNameList.size();

    for (unsigned i = 0; i < n; ++i) {
        parametrization->setParameter(_parameterNameList[i], vector[i]);
    }

    return parametrization;
}

Parametrization::Ptr ParametrizationTranslator::vectorToParametrization(const ValueVector& vector) {
    Parametrization::Ptr parametrization = ownedPtr(new Parametrization(_parameterNameList));

    return applyVectorToParametrization(parametrization, vector);
}

