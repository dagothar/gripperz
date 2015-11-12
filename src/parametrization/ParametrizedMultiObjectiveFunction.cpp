/* 
 * File:   ParametrizedMultiObjectiveFunction.pp
 * Author: dagothar
 *
 * Created on November 12, 2015, 11:18 AM
 */

#include "ParametrizedMultiObjectiveFunction.hpp"

using namespace gripperz::parametrization;
using namespace gripperz::math;

ParametrizedMultiObjectiveFunction::ParametrizedMultiObjectiveFunction(ParametrizationTranslator::Ptr translator) :
ParametrizedObjective(translator){
}

ParametrizedMultiObjectiveFunction::~ParametrizedMultiObjectiveFunction() {
}

Vector ParametrizedMultiObjectiveFunction::evaluate(const Vector& x) {
    Parametrization::Ptr parametrization = translate(x);
    return doEvaluate(parametrization);
}
