/* 
 * File:   MultiParametrizationTranslator.cpp
 * Author: dagothar
 *
 * Created on November 6, 2015, 10:59 AM
 */

#include "MultiParametrizationTranslator.hpp"
#include <rw/common/macros.hpp>
#include <vector>

using namespace gripperz::parametrization;
using namespace std;
using namespace rw::common;

MultiParametrizationTranslator::MultiParametrizationTranslator(const std::vector<Slot>& slots) :
_slots(slots) {
}

MultiParametrizationTranslator::MultiParametrizationTranslator(std::initializer_list<Slot> slots) :
_slots(slots) {
}

MultiParametrizationTranslator::~MultiParametrizationTranslator() {
}

ParametrizationTranslator::ValueVector MultiParametrizationTranslator::parametrizationToVector(Parametrization::Ptr parametrization) {
    unsigned n = _slots.size();
    ValueVector values(n);

    for (unsigned i = 0; i < n; ++i) {
        Slot& slot = _slots[i];
        const vector<string>& names = slot.getParameterNames();

        RW_ASSERT(names.size() > 0);

        values[i] = parametrization->getParameter(names[0]);
    }

    return values;
}

Parametrization::Ptr MultiParametrizationTranslator::vectorToParametrization(const ValueVector& vector) {
    Parametrization::Ptr parametrization = ownedPtr(new Parametrization(getParameterNameList()));

    return applyVectorToParametrization(parametrization, vector);
}

Parametrization::Ptr MultiParametrizationTranslator::applyVectorToParametrization(Parametrization::Ptr parametrization, const ValueVector& vector) {
    RW_ASSERT(_slots.size() == vector.size());

    for (unsigned i = 0; i < _slots.size(); ++i) {

        const std::vector<string>& names = _slots[i].getParameterNames();

        for (unsigned j = 0; j < names.size(); ++j) {
            parametrization->setParameter(names[j], vector[i]);
        }
    }

    return parametrization;
}

std::vector<std::string> MultiParametrizationTranslator::getParameterNameList() const {
    vector<string> names;

    for (unsigned i = 0; i < _slots.size(); ++i) {

        const vector<string>& n = _slots[i].getParameterNames();

        for (unsigned j = 0; j < n.size(); ++j) {
            names.push_back(n[j]);
        }
    }

    return names;
}
