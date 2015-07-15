/**
 * @file GripperOptimizationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#include "GripperOptimizationManager.hpp"


using namespace gripperz::optimization;


GripperOptimizationManager::GripperOptimizationManager(Optimizer::Ptr optimizer) :
	_optimizer(optimizer)
{
}


GripperOptimizationManager::~GripperOptimizationManager() {
}
