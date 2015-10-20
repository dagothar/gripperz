/**
 * @file Context.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "Context.hpp"


using namespace gripperz::context;

Context::Context(rwsim::dynamics::DynamicWorkCell::Ptr dwc) :
_dynamicWorkCell(dwc) {
    _workCell = _dynamicWorkCell->getWorkcell();
    _initState = _workCell->getDefaultState();
}

Context::~Context() {
}
