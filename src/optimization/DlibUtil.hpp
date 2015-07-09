/**
 * @file DlibUtil.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#pragma once

#include <rw/common/Ptr.hpp>
#include <dlib/optimization.h>

namespace gripperz {
namespace optimization {

/**
 * @class DlibUtil
 * @brief Utilities for interfacing with Dlib
 */
class DlibUtil {
public:
	//! Converts parameter vector into dlib vector.
	static dlib::matrix<double, 0, 1> vectorToDlib(const std::vector<double>& vec);
	
	//! Converts dlib vector into parameter vector.
	static std::vector<double> dlibToVector(const dlib::matrix<double, 0, 1>& vec);
};

} /* optimization */
} /* gripperz */
