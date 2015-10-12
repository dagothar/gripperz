/**
 * @file GripperQuality.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-13
 */

#include "OldGripperQuality.hpp"


using namespace gripperz::models;
using namespace std;


OldGripperQuality::OldGripperQuality() :
	success(0),
	robustness(0),
	alignment(0),
	coverage(0),
	wrench(0),
	topwrench(0),
	stress(0),
	volume(0),
	quality(0)
{
}


OldGripperQuality::~OldGripperQuality() {
}


std::ostream& gripperz::models::operator<<(std::ostream& stream, const OldGripperQuality& q) {
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