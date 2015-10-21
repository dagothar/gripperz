/* 
 * File:   ParametrizedMeshGenerator.cpp
 * Author: dagothar
 * 
 * Created on October 21, 2015, 3:10 PM
 */

#include "ParametrizedMeshGenerator.hpp"

using namespace gripperz::geometry;
using namespace gripperz::parametrization;

ParametrizedMeshGenerator::ParametrizedMeshGenerator(Parametrization::Ptr parametrization) :
Parametrized(parametrization) {
}

ParametrizedMeshGenerator::~ParametrizedMeshGenerator() {
}

