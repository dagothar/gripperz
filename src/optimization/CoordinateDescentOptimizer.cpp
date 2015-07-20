/**
 * @file CoordinateDescentOptimizer.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-19
 */

#include "CoordinateDescentOptimizer.hpp"
#include <rw/math/Q.hpp>
#include <rw/math/Math.hpp>

#define DEBUG rw::common::Log::infoLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::optimization;
using namespace std;
using namespace rw::math;
using namespace gripperz::math;


CoordinateDescentOptimizer::CoordinateDescentOptimizer(const Configuration& conf) :
	_conf(conf)
{
}


CoordinateDescentOptimizer::~CoordinateDescentOptimizer() {
}


Vector CoordinateDescentOptimizer::minimize(math::ObjectiveFunction::Ptr function, const Vector& initialGuess) {
	unsigned dim = initialGuess.size();
	
	/* intialize data */
	unsigned fev = 0;
	Scalar fbest = function->evaluate(initialGuess); ++fev;
	
	Q m(dim);
	m = Math::fromStdVector(initialGuess, m);
	
	/* initialize step sizes */
	Q stepSizes(dim);
	for (unsigned i = 0; i < dim; ++i) stepSizes(i) = _conf.kInitialStepSize;
	
	bool stop = false;
	unsigned ix = 0;
	while (!stop) {
		/* pick step */
		Q step(dim);
		for (unsigned i = 0; i < dim; ++i) step(i) = 0.0;
		step(ix) = stepSizes(ix);
		
		/* try - */
		Q qx1 = m - step;
		Vector x1 = Math::toStdVector(qx1, dim);
		Scalar f1 = function->evaluate(x1); ++fev;
		
		/* try + */
		Q qx2 = m + step;
		Vector x2 = Math::toStdVector(qx2, dim);
		Scalar f2 = function->evaluate(x2); ++fev;
		
		bool success = 0;
		
		/* check improvement */
		if (f1 < fbest) {
			fbest = f1;
			m = qx1;
			success = true;
			DEBUG << "f1 < fbest" << endl;
		}
		if (f2 < fbest) {
			fbest = f2;
			m = qx2;
			success = true;
			DEBUG << "f2 < fbest" << endl;
		}
		
		DEBUG << "m= " << m << endl;
		
		/* update step size */
		if (success) {
			stepSizes(ix) *= _conf.kSuccessful;
		} else {
			stepSizes(ix) *= _conf.kUnsuccesful;
		}
		
		double rho = stepSizes.norm2();
		
		DEBUG << " rho=" << rho << " fev=" << fev << endl;
		
		/* check stopping condition */
		if (
			rho < _conf.kFinalStepSize
			|| fev > _conf.maxNOfEvaluations
		) {
			stop = true;
		}
		
		/* pick dimension */
		ix = (ix + 1) % dim;
	}
	
	Vector result = Math::toStdVector(m, dim);
	return result;
}
