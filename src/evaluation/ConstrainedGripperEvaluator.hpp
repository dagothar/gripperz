/**
 * @file ConstrainedGripperEvaluator.hpp
 * @author Adam Wolniakowski
 * @date 13-07-2015
 */

#pragma once

#include "GripperEvaluator.hpp"

namespace gripperz {
namespace evaluation {
	
/**
 * @class ConstrainedGripperEvaluator
 * @brief Adds precondition on values of stress and volume.
 */
class ConstrainedGripperEvaluator: public GripperEvaluator {
public:
	typedef rw::common::Ptr<ConstrainedGripperEvaluator> Ptr;

public:
	/**
	 * @brief Constructor.
	 * @param context [in] task description
	 */
	ConstrainedGripperEvaluator(context::TaskDescription::Ptr context, AlignmentCalculator::Ptr alignmentCalculator);
	
	virtual ~ConstrainedGripperEvaluator();

	/**
	 * @brief Tests sanity of the gripper design.
	 */
	virtual bool isSane(models::Gripper::Ptr gripper);
};

} /* evaluation */
} /* gripperz */
