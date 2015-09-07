/**
 * @file ClusteringAlignment.hpp
 * @author Adam Wolniakowski
 * @date 2015-08-15
 */

#pragma once

#include "AlignmentCalculator.hpp"

namespace gripperz {
namespace evaluation {

/**
 * @class ClusteringAlignment
 * @brief Calculates alignment based on X/Y/Z versor clustering
 */
class ClusteringAlignment: public virtual AlignmentCalculator {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<ClusteringAlignment> Ptr;
	
	//! Configuration for clustering alignment.
	struct Configuration {
		double filteringRadius;
		
		Configuration() :
			filteringRadius(0.1)
		{}
	};

public:
	/**
	 * @brief Constructor.
	 * @param conf [in] parameters for finding point model with RANSAC
	 * @param c1 [in] parameters for finding stable pose 1D model with RANSAC
	 */
	ClusteringAlignment(const Configuration& conf=Configuration());
	
	virtual ~ClusteringAlignment();
	
	//! @copydoc AlignmentCalculator::calculateAlignment
	virtual double calculateAlignment(rwlibs::task::GraspTask::Ptr tasks);

protected:
	/**
	 * @brief Adjusts the filtering radius according to the number of targets.
	 */
	virtual double adjustRadius(double radius, double ntargets);

private:
	Configuration _conf;
};

} /* evaluation */
} /* gripperz */
