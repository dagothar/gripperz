/**
 * @file Types.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-14
 */

#include <vector>
#include <iostream>
#include <string>

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

/**
 * @brief Converts Vector type to a string.
 */
std::string toString(const Vector& vec);

/**
 * @brief Writes Vector to a stream.
 */
std::ostream& operator<<(std::ostream& stream, const Vector& vector);
	
} /* math */
} /* gripperz */
