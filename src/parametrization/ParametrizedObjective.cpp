/* 
 * File:   ParametrizedObjective.cpp
 * Author: dagothar
 *
 * Created on November 12, 2015, 11:08 AM
 */

#include "ParametrizedObjective.hpp"
#include <rw/common/macros.hpp>

using namespace gripperz::parametrization;

ParametrizedObjective::ParametrizedObjective(ParametrizationTranslator::Ptr translator) :
_translator(translator) {
}

ParametrizedObjective::~ParametrizedObjective() {
}

Parametrization::Ptr ParametrizedObjective::translate(const math::Vector& vector) {
    RW_ASSERT(_translator);
    
    return _translator->vectorToParametrization(vector);
}
