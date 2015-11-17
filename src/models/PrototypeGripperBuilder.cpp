/* 
 * File:   PrototypeGripperBuilder.cpp
 * Author: dagothar
 *
 * Created on November 17, 2015, 10:33 AM
 */

#include "PrototypeGripperBuilder.hpp"

using namespace gripperz::models;
using namespace gripperz::math;
using namespace rw::common;

PrototypeGripperBuilder::PrototypeGripperBuilder(ParametrizedGripper::Ptr prototype, parametrization::ParametrizationTranslator::Ptr translator) :
_prototype(prototype),
_translator(translator) {

}

PrototypeGripperBuilder::~PrototypeGripperBuilder() {
}

ParametrizedGripper::Ptr PrototypeGripperBuilder::vectorToGripper(const Vector& params) {
    ParametrizedGripper::Ptr gripper = ownedPtr(_prototype->clone());
}

Vector PrototypeGripperBuilder::gripperToVector(ParametrizedGripper::Ptr gripper) {
    RW_ASSERT(_translator != NULL);
    
    Vector x = _translator->parametrizationToVector(gripper->getParametrization());
    return x;
}
