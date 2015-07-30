/**
 * @file RobustEvaluationManager.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-30
 */

#pragma once

#include "GripperEvaluationManager.hpp"

namespace gripperz {
namespace evaluation {

/**
 * @class RobustEvaluationManager
 * @brief Performs robust evaluation by delegating to a script running separately.
 * In case of crash, evaluation is re-tried, until succesful, or a limit on
 * number of evaluations reached.
 */
class RobustEvaluationManager: public GripperEvaluationManager {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<RobustEvaluationManager> Ptr;

public:
	RobustEvaluationManager(const Configuration& config);
	
	virtual ~RobustEvaluationManager();
	
	virtual models::GripperQuality::Ptr evaluateGripper(models::Gripper::Ptr gripper);
};

} /* evaluation */
} /* gripperz */
