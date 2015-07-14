/**
 * @file CombineObjectivesFactory.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "CombineObjectives.hpp"

namespace gripperz {
namespace math {

/**
 * @class CombineObjectivesFactory
 * @brief Factory for methods combining objectives.
 */
class CombineObjectivesFactory {
public:
	/**
	 * @brief Creates an objective combining method.
	 * @param method [in] method name (sum, product, log)
	 * @param weights [in] weights
	 * @return objective combining method
	 */
	static CombineObjectives::Ptr make(const std::string& method, const std::vector<double>& weights = std::vector<double>());
};

} /* math */
} /* gripperz */
