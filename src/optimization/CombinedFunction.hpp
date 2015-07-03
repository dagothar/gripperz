/**
 * @file CombinedFunction.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "ObjectiveFunction.hpp"
#include "MultiObjectiveFunction.hpp"
#include "CombineObjectives.hpp"

namespace gripperz {
namespace optimization {
	
/**
 * @class CombinedFunction
 * @brief Interface for single objective functions combined from multi objective functions.
 */
class CombinedFunction : virtual public ObjectiveFunction {
public:
	typedef rw::common::Ptr<CombinedFunction> Ptr;
	
public:
	CombinedFunction(MultiObjectiveFunction::Ptr multiObjFunc, CombineObjectives::Ptr combineMethod);
	virtual ~CombinedFunction();
	
	CombineObjectives::Ptr getCombiningMethod() const { return _method; }
	void setCombiningMethod(CombineObjectives::Ptr method) { _method = method; }
	
	/**
	 * @brief Returns evaluation of multi-objective function combined using the combining method.
	 */
	virtual double operator()(const std::vector<double>& x);

private:
	MultiObjectiveFunction::Ptr _multiObjFunc;
	CombineObjectives::Ptr _method;
};

} /* optimization */
} /* gripperz */
