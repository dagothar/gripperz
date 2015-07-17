/**
 * @file AllPosesAlignment.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-17
 */

#pragma once

#include "AlignmentCalculator.hpp"

namespace gripperz {
namespace evaluation {

/**
 * @class AllPosesAlignment
 * @brief Calculates alignment based on the distance travelled for ALL poses.
 * This doesn't work well with cut tilt -> yields consistently bigger results.
 */
class AllPosesAlignment: public virtual AlignmentCalculator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<AllPosesAlignment> Ptr;

public:
	AllPosesAlignment();
	
	virtual ~AllPosesAlignment();
	
	//! @copydoc AlignmentCalculator::calculateAlignment
	virtual double calculateAlignment(rwlibs::task::GraspTask::Ptr tasks);
};

} /* evaluation */
} /* gripperz */
