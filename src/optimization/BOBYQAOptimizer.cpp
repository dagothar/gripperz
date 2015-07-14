/**
 * @file BOBYQAOptimizer.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#include "BOBYQAOptimizer.hpp"
#include "DlibWrapper.hpp"
#include "DlibUtil.hpp"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <rw/common/macros.hpp>


using namespace gripperz::optimization;
using namespace rw::common;
using namespace std;


BOBYQAOptimizer::BOBYQAOptimizer(Configuration config, const ConstraintList& constr) :
	_config(config),
	_constraints(constr)
{
}


BOBYQAOptimizer::~BOBYQAOptimizer() {
}


std::vector<double> BOBYQAOptimizer::minimize(ObjectiveFunction::Ptr function, const std::vector<double>& initialGuess) {
	/* create dlib function to optimize */
	DlibFunction::Ptr dlibFunc = ownedPtr(new DlibWrapper(function));
	boost::function<double(const dlib::matrix<double, 0, 1>&)> func = boost::bind(&DlibFunction::evaluate, dlibFunc.get(), _1);
	
	/* translate initial guess */
	dlib::matrix<double, 0, 1> init = DlibUtil::vectorToDlib(initialGuess);
	
	/* translate constraints */
	vector<double> lower, upper;
	BOOST_FOREACH (Constraint& c, _constraints) {
		lower.push_back(c.first);
		upper.push_back(c.second);
	}
	
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
	
	vector<double> result = DlibUtil::dlibToVector(init);
	return result;
}
