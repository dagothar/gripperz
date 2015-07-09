/**
 * @file DlibFunction.hpp
 * @author Adam Wolniakowski
 * @date 9-07-2015
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <dlib/optimization.h>

namespace gripperz {
namespace optimization {
	
/**
 * @class DlibFunction
 * @brief Dlib function interface.
 */
class DlibFunction {
public:
	typedef rw::common::Ptr<DlibFunction> Ptr;
	
public:
	//! Returns evaluation of provided vector x.
	virtual double operator()(const dlib::matrix<double, 0, 1>& x) = 0;
};

} /* optimization */
} /* gripperz */
