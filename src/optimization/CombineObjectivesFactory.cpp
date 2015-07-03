/**
 * @file CombineObjectivesFactory.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "CombineObjectivesFactory.hpp"

#include <rw/common/macros.hpp>
#include "CombineSum.hpp"
#include "CombineProduct.hpp"
#include "CombineLog.hpp"


using namespace gripperz::optimization;
using namespace rw::common;


CombineObjectives::Ptr CombineObjectivesFactory::make(const std::string& method, const std::vector<double>& weights) {
	if (method == "sum") {
		return ownedPtr(new CombineSum(weights));
	}
	else if (method == "product") {
		return ownedPtr(new CombineProduct(weights));
	}
	else if (method == "log") {
		return ownedPtr(new CombineLog(weights));
	}
	else {
		RW_THROW("Objective combining method is not supported: " << method);
	}
}
