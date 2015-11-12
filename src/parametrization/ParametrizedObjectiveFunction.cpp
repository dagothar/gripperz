/* 
 * File:   ParametrizedObjectiveFunction.cpp
 * Author: dagothar
 *
 * Created on November 12, 2015, 10:59 AM
 */

#include "ParametrizedObjectiveFunction.hpp"
#include "ParametrizedObjective.hpp"

using namespace gripperz::parametrization;
using namespace gripperz::math;

ParametrizedObjectiveFunction::ParametrizedObjectiveFunction(ParametrizationTranslator::Ptr translator) :
ParametrizedObjective(translator) {
}


ParametrizedObjectiveFunction::~ParametrizedObjectiveFunction() {
}

Scalar ParametrizedObjectiveFunction::evaluate(const Vector& x) {
    Parametrization::Ptr parametrization = translate(x);
    
    return doEvaluate(parametrization);
}
