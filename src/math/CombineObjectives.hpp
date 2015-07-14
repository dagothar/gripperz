/**
 * @file CombineObjectives.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "Types.hpp"
#include <rw/common/Ptr.hpp>

namespace gripperz {
namespace math {

/**
 * @class CombineObjectives
 * @brief Interface for methods combining objectives.
 */
class CombineObjectives {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<CombineObjectives> Ptr;
	
public:
	CombineObjectives(const Vector& weights = Vector());
	virtual ~CombineObjectives();
	
	const Vector& getWeights() const;
	void setWeights(const Vector& weights);
	
	/**
	 * @brief Combines objectives.
	 * @param objectives [in] a vector of individual objectives
	 * @return combined quality
	 */
	Scalar combine(const Vector& objectives);

protected:
	virtual Scalar _do_combine(const Vector& objectives) = 0;
	
private:
	Vector _weights;
};

} /* math */
} /* gripperz */
