/**
 * @file OptimizationManager.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#include "OptimizationManager.hpp"
#include <math/RevertedFunction.hpp>
#include <math/MappedFunction.hpp>
#include <math/CallbackFunction.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::optimization;
using namespace gripperz::math;


OptimizationManager::OptimizationManager(Optimizer::Ptr opt, const RangeList& ranges) :
	_optimizer(opt),
	_ranges(ranges)
{
}


OptimizationManager::~OptimizationManager() {
}


Vector OptimizationManager::optimize(ObjectiveFunction::Ptr function, const Vector& initialGuess, const std::string& dir) {
	ObjectiveFunction::Ptr objective = function;
	
	/* clear log */
	_log.clear();
	
	/* first wrap function in the callback */
	objective = new CallbackFunction(function, boost::bind(&OptimizationManager::callback, this, _1, _2));
	
	/* take care of direction */
	if (dir == "maximize") {
		objective = new RevertedFunction(objective);
	}
	
	/* map function to normalized box */
	ParameterMapping::Map map;
	if (_ranges.size() > 0) {
		BOOST_FOREACH (Range& range, _ranges) {
			map.push_back({{0, 1}, range});
		}
	} else {
		for (unsigned i = 0; i < initialGuess.size(); ++i) {
			map.push_back({{0, 1}, {0, 1}});
		}
	}
	
	ParameterMapping::Ptr mapping = new ParameterMapping(map);
	objective = new MappedFunction(objective, mapping);
	
	/* perform actual optimization */
	Vector result = _optimizer->minimize(objective, mapping->unmap(initialGuess));
	
	return mapping->map(result);
}


void OptimizationManager::callback(const Vector& args, Scalar result) {
	_log.push_back({args, result});
	
	if (_userCallback) {
		_userCallback(args, result);
	}
}
