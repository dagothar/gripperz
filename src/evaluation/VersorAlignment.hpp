/**
 * @file VersorAlignment.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-17
 */

#pragma once

#include "AlignmentCalculator.hpp"

namespace gripperz {
namespace evaluation {

/**
 * @class VersorAlignment
 * @brief Calculates alignment based on X/Y/Z versor clustering
 */
class VersorAlignment: public virtual AlignmentCalculator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<VersorAlignment> Ptr;
	
	//! Configuration for finding a point model.
	struct Configuration {
		int iterations;
		int minInliers;
		double dataThreshold;
		double modelThreshold;
		double weight;
		
		Configuration() :
			iterations(100),
			minInliers(5),
			dataThreshold(0.1),
			modelThreshold(1.0),
			weight(0.5)
		{}
	};

public:
	/**
	 * @brief Constructor.
	 * @param conf [in] parameters for finding point model with RANSAC
	 * @param c1 [in] parameters for finding stable pose 1D model with RANSAC
	 */
	VersorAlignment(const Configuration& conf=Configuration());
	
	virtual ~VersorAlignment();
	
	//! @copydoc AlignmentCalculator::calculateAlignment
	virtual double calculateAlignment(rwlibs::task::GraspTask::Ptr tasks);

private:
	Configuration _conf;
};

} /* evaluation */
} /* gripperz */
