/**
 * @file BOBYQAOptimizer.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#include "BOBYQAOptimizer.hpp"
#include <math/DlibWrapper.hpp>
#include <math/MappedFunction.hpp>
#include <util/DlibUtil.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <rw/common/macros.hpp>


using namespace gripperz::optimization;
using namespace gripperz::math;
using namespace gripperz::util;
using namespace rw::common;
using namespace std;


BOBYQAOptimizer::BOBYQAOptimizer(Configuration config, const ConstraintList& constr) :
	_config(config),
	_constraints(constr)
{
	_mapping = makeMapping(constr);
}


BOBYQAOptimizer::~BOBYQAOptimizer() {
}


void BOBYQAOptimizer::setConstraints(const ConstraintList& constr) {
	_constraints = constr;
	_mapping = makeMapping(_constraints);
}


ParameterMapping::Ptr BOBYQAOptimizer::makeMapping(const ConstraintList& constr) {
	ParameterMapping::Map map;
	
	for (unsigned i = 0; i < constr.size(); ++i) {
		map.push_back({{0, 1}, constr[i]});
	}
	
	return ownedPtr(new ParameterMapping(map));
}


Vector BOBYQAOptimizer::minimize(ObjectiveFunction::Ptr function, const Vector& initialGuess) {
	/* create mapped function */
	ObjectiveFunction::Ptr mapped = new MappedFunction(function, _mapping);
	
	/* create dlib function to optimize */
	DlibFunction::Ptr dlibFunc = ownedPtr(new DlibWrapper(function));
	boost::function<double(const dlib::matrix<double, 0, 1>&)> func = boost::bind(&DlibFunction::evaluate, dlibFunc.get(), _1);
	
	/* translate initial guess */
	dlib::matrix<double, 0, 1> init = DlibUtil::vectorToDlib(_mapping->unmap(initialGuess));
	
	/* translate constraints */
	Vector lower(initialGuess.size(), 0);
	Vector upper(initialGuess.size(), 1);
	
	/* perform optimization */
	try {
		dlib::find_min_bobyqa<
			boost::function<double(const dlib::matrix<double, 0, 1>&)>,
			dlib::matrix<double, 0, 1>,
			dlib::matrix<double, 0, 1>
		>(
			func, // function to minimize
			init,
			init.size() * 2 + 1, // number of interpolation points
			DlibUtil::vectorToDlib(lower),
			DlibUtil::vectorToDlib(upper),
			_config.initialTrustRegionRadius,
			_config.finalTrustRegionRadius,
			_config.maxNOfEvaluations
		);
	} catch (dlib::bobyqa_failure& e) {
		RW_THROW ("Exception during dlib optimization: " << e.what());
	}
	
	Vector result = _mapping->map(DlibUtil::dlibToVector(init));
	return result;
}
