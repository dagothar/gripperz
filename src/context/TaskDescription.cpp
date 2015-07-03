#include "TaskDescription.hpp"


#define DEBUG rw::common::Log::debugLog()



using namespace std;
USE_ROBWORK_NAMESPACE;
using namespace robwork;
using namespace rwsim;
using namespace rwsim::dynamics;
using namespace gripperz::util;
using namespace gripperz::context;



TaskDescription::TaskDescription(rwsim::dynamics::DynamicWorkCell::Ptr dwc) :
	_dwc(dwc),
	_wc(dwc->getWorkcell().get()),
	_initState(_wc->getDefaultState()),
	_interferenceLimit(0.0),
	_wrenchLimit(0.0),
	_stressLimit(10.0), // arbitrary
	_targetObject(NULL),
	_stablePose0DParameters(100, 10, 0.5, 0.5, 0.5),
	_stablePose1DParameters(100, 10, 0.5, 0.5, 0.5)
{
	if (_wc == NULL || _dwc == NULL) {
		RW_THROW("NULL WC or DWC!");
	}
}


TaskDescription::~TaskDescription()
{
}
