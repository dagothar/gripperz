/**
 * @file Types.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#include <vector>

namespace gripperz {
namespace math {
	
//! A type for scalar math
typedef double Scalar;

//! A type for math vector.
typedef std::vector<Scalar> Vector;

//! A type for range.
typedef std::pair<Scalar, Scalar> Range;

//! A list of ranges.
typedef std::vector<Range> RangeList;
	
} /* math */
} /* gripperz */
