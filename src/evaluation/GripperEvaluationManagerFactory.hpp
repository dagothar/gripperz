/**
 * @file GripperEvaluationManagerFactory.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include "GripperEvaluationManager.hpp"
#include "StandardEvaluationManager.hpp"
#include <grasps/SurfaceSample.hpp>

namespace gripperz {
namespace evaluation {

/**
 * @class GripperEvaluationManagerFactory
 * @brief
 */
class GripperEvaluationManagerFactory {
public:
	static StandardEvaluationManager::Ptr makeStandardEvaluationManager(
		context::TaskDescription::Ptr td,
		const GripperEvaluationManager::Configuration& config,
		unsigned nThreads=1,
		const std::vector<grasps::SurfaceSample>& ssamples=std::vector<grasps::SurfaceSample>()
	);
};

} /* evaluation */
} /* gripperz */
