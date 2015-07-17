#include "TaskDescription.hpp"
#include <evaluation/StablePoseAlignment.hpp>


#define DEBUG rw::common::Log::debugLog()



using namespace std;
USE_ROBWORK_NAMESPACE;
using namespace robwork;
using namespace rwsim;
using namespace rwsim::dynamics;
using namespace gripperz::util;
using namespace gripperz::context;
using namespace gripperz::evaluation;



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


AlignmentCalculator::Ptr TaskDescription::getAlignmentCalculator() {
	
	StablePoseAlignment::Configuration conf0;
	conf0.iterations = _stablePose0DParameters.iterations;
	conf0.minInliers = _stablePose0DParameters.minInliers;
	conf0.dataThreshold = _stablePose0DParameters.dataThreshold;
	conf0.modelThreshold = _stablePose0DParameters.modelThreshold;
	conf0.weight = _stablePose0DParameters.weight;
	
	StablePoseAlignment::Configuration conf1;
	conf1.iterations = _stablePose1DParameters.iterations;
	conf1.minInliers = _stablePose1DParameters.minInliers;
	conf1.dataThreshold = _stablePose1DParameters.dataThreshold;
	conf1.modelThreshold = _stablePose1DParameters.modelThreshold;
	conf1.weight = _stablePose1DParameters.weight;
	
	AlignmentCalculator::Ptr calc = ownedPtr(
		new StablePoseAlignment(conf0, conf1)
	);
	
	return calc;
}
