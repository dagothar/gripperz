/**
 * @file Types.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#include "Types.hpp"
#include <sstream>
#include <boost/foreach.hpp>


using namespace gripperz::math;
using namespace std;


std::string gripperz::math::toString(const Vector& vec) {
	stringstream sstr;
	
	for (Vector::const_iterator i = vec.begin(); i != vec.end(); ++i) {
		sstr << *i;
		
		if (i != vec.end() - 1) sstr << ", ";
	}
	
	return sstr.str();
}


std::ostream& gripperz::math::operator<<(std::ostream& stream, const Vector& vector) {
	stream << toString(vector);
	
	return stream;
}
