/**
 * @file InterferenceSimulator.cpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include "InterferenceSimulator.hpp"

#include <iostream>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::simulator;
using namespace rwlibs::task;
using namespace std;


InterferenceSimulator::InterferenceSimulator(rwsim::dynamics::DynamicWorkCell::Ptr dwc, unsigned threads) :
	BasicSimulator(dwc, threads)
{
}


InterferenceSimulator::~InterferenceSimulator() {
}


void InterferenceSimulator::graspFinished(SimState& sstate) {
	cout << "Calculating interference" << endl;
	
	BasicSimulator::graspFinished(sstate);
}
