/**
 * @file DlibWrapper.hpp
 * @author Adam Wolniakowski
 * @date 9-07-2015
 */

#pragma once

#include "DlibFunction.hpp"
#include "ObjectiveFunction.hpp"

namespace gripperz {
namespace optimization {
	
/**
 * @class DlibFunction
 * @brief Dlib function interface.
 */
class DlibWrapper: public DlibFunction {
public:
	typedef rw::common::Ptr<DlibWrapper> Ptr;
	
public:
	/**
	 * @brief Constructor.
	 * 
	 * Wraps ObjectiveFunction into a DlibFunction.
	 */
	DlibWrapper(ObjectiveFunction::Ptr func);
	virtual ~DlibWrapper();
	
	ObjectiveFunction::Ptr getObjectiveFunction() { return _objFunc; }
	void setObjectiveFunction(ObjectiveFunction::Ptr func) { _objFunc = func; }
	
	//! Returns evaluation of provided vector x.
	virtual double operator()(const dlib::matrix<double, 0, 1>& x);

private:
	ObjectiveFunction::Ptr _objFunc;
};

} /* optimization */
} /* gripperz */
