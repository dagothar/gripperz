/**
 * @file GripperQuality.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-13
 */

#include "GripperQuality.hpp"


using namespace gripperz::models;
using namespace std;


GripperQuality::GripperQuality() {
}


GripperQuality::~GripperQuality() {
}


std::ostream& gripperz::models::operator<<(std::ostream& stream, const GripperQuality& q) {
	stream <<
		"GripperQuality:\n" <<
		" - success = " << q.success << "\n" <<
		" - robustness = " << q.robustness << "\n" <<
		" - alignment = " << q.alignment << "\n" <<
		" - coverage = " << q.coverage << "\n" <<
		" - wrench = " << q.wrench << "\n" <<
		" - topwrench = " << q.topwrench << "\n" <<
		" - stress = " << q.stress << "\n" <<
		" - volume = " << q.volume << "\n" <<
		" - QUALITY = " << q.quality << "\n" << endl;

	return stream;
}
