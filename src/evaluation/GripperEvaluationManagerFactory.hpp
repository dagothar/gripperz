/**
 * @file GripperEvaluationManagerFactory.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include "GripperEvaluationManager.hpp"

namespace gripperz {
namespace evaluation {

/**
 * @class GripperEvaluationManagerFactory
 * @brief
 */
class GripperEvaluationManagerFactory {
public:
	static GripperEvaluationManager::Ptr getEvaluationManager(context::TaskDescription::Ptr td, unsigned nTargets=100);
};

} /* evaluation */
} /* gripperz */
