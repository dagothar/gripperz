#include "TaskDescription.hpp"
#include <evaluation/StablePoseAlignment.hpp>
#include <evaluation/VersorAlignment.hpp>
#include <evaluation/SimpleAlignment.hpp>
#include <evaluation/ClusteringAlignment.hpp>


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
	_alignmentParameters(1000, 5, 0.05, 0.1, 5)
{
	if (_wc == NULL || _dwc == NULL) {
		RW_THROW("NULL WC or DWC!");
	}
}


TaskDescription::~TaskDescription()
{
}


AlignmentCalculator::Ptr TaskDescription::getAlignmentCalculator() {
	ClusteringAlignment::Configuration conf;
	conf.filteringRadius = _alignmentFilteringDistance;
	
	AlignmentCalculator::Ptr calc = ownedPtr(
		new ClusteringAlignment(conf)
	);
	
	return calc;
}
