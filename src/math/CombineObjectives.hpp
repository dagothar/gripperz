/**
 * @file CombineObjectives.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <vector>
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
	CombineObjectives(const std::vector<double> weights = std::vector<double>());
	virtual ~CombineObjectives();
	
	const std::vector<double>& getWeights() const;
	void setWeights(const std::vector<double>& weights);
	
	/**
	 * @brief Combines objectives.
	 * @param objectives [in] a vector of individual objectives
	 * @return combined quality
	 */
	double combine(const std::vector<double>& objectives);

protected:
	virtual double _do_combine(const std::vector<double>& objectives) = 0;
	
private:
	std::vector<double> _weights;
};

} /* math */
} /* gripperz */
