/**
 * @file StablePoseAlignment.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-17
 */

#pragma once

#include "AlignmentCalculator.hpp"
#include <context/TaskDescription.hpp>

namespace gripperz {
namespace evaluation {

/**
 * @class StablePoseAlignment
 * @brief Calculates alignment based on stable poses found by RANSAC
 */
class StablePoseAlignment: public virtual AlignmentCalculator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<StablePoseAlignment> Ptr;
	
	//! Configuration for finding a stable pose model.
	struct Configuration {
		int iterations;
		int minInliers;
		double dataThreshold;
		double modelThreshold;
		double weight;
		
		Configuration() :
			iterations(100),
			minInliers(10),
			dataThreshold(0.25),
			modelThreshold(1.0),
			weight(0.5)
		{}
	};

public:
	/**
	 * @brief Constructor.
	 * @param c0 [in] parameters for finding stable pose 0D model with RANSAC
	 * @param c1 [in] parameters for finding stable pose 1D model with RANSAC
	 */
	StablePoseAlignment(const Configuration& c0=Configuration(), const Configuration& c1=Configuration());
	
	virtual ~StablePoseAlignment();
	
	//! @copydoc AlignmentCalculator::calculateAlignment
	virtual double calculateAlignment(rwlibs::task::GraspTask::Ptr tasks);

private:
	Configuration _config0;
	Configuration _config1;
};

} /* evaluation */
} /* gripperz */
