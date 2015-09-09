/**
 * @file PowellOptimizer.cpp
 * @author Adam Wolniakowski
 * @date 2015-09-07
 */

#include "PowellOptimizer.hpp"
#include <rw/math/Q.hpp>
#include <rw/math/Math.hpp>
#include <boost/foreach.hpp>

#define DEBUG rw::common::Log::infoLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::optimization;
using namespace std;
using namespace rw::math;
using namespace gripperz::math;


PowellOptimizer::PowellOptimizer(const Configuration& config) :
	_config(config)
{
}


PowellOptimizer::~PowellOptimizer() {
}

bool directionComp(const Q& dir1, const Q& dir2) {
	return (dir1.norm2() < dir2.norm2());
}


Vector PowellOptimizer::minimize(math::ObjectiveFunction::Ptr function, const Vector& initialGuess) {
	unsigned dim = initialGuess.size();
	
	DEBUG << "dim: " << dim << endl;
	
	/* intialize data */
	int fev = 0;
	Scalar best_value = function->evaluate(initialGuess); ++fev;
	
	Q current_guess(dim);
	current_guess = Math::fromStdVector(initialGuess, current_guess);
	
	/* initialize direction set */
	vector<Q> dir_set;
	for (unsigned i = 0; i < dim; ++i) {
		Q base_dir(dim, 0.0);
		base_dir(i) = _config.rho_0;
		
		DEBUG << "base dir(" << i << "):" << base_dir << endl;

		dir_set.push_back(base_dir);
	}
	
	bool stop = false;
	unsigned dir_idx = 0;
	while (!stop) {
		/* pick direction */
		Q& step = dir_set[dir_idx];
		
		DEBUG << "step: " << step << endl;
		
		/* try - */
		Q qx1 = current_guess - step;
		Vector x1 = Math::toStdVector(qx1, dim);
		Scalar f1 = function->evaluate(x1); ++fev;
		
		/* try + */
		Q qx2 = current_guess + step;
		Vector x2 = Math::toStdVector(qx2, dim);
		Scalar f2 = function->evaluate(x2); ++fev;
		
		bool success = 0;
		
		/* check improvement */
		if (f1 < best_value) {
			best_value = f1;
			current_guess = qx1;
			success = true;
			DEBUG << "f1 < fbest" << endl;
		}
		if (f2 < best_value) {
			best_value = f2;
			current_guess = qx2;
			success = true;
			DEBUG << "f2 < fbest" << endl;
		}
		
		DEBUG << "current_guess= " << current_guess << endl;
		
		/* update step size */
		if (success) {
			dir_set[dir_idx] *= _config.kSuccessful;
		} else {
			dir_set[dir_idx] *= _config.kUnsuccesful;
		}
		
		double rho = 0.0;
		BOOST_FOREACH(Q& dir, dir_set) {
			rho += dir.norm2();
		}
		rho /= dir_set.size();
		
		DEBUG << " rho=" << rho << " fev=" << fev << " fbest= " << best_value << endl;
		
		/* pick direction */
		++dir_idx;
		if (dir_idx >= dir_set.size()) {
			// sort directions in ascending order
			std::sort(dir_set.begin(), dir_set.end(), directionComp);

			// create new direction
			Q new_dir(dim, 0.0);
			//double totalQuality = 0.0;
			BOOST_FOREACH(Q& dir, dir_set) {
				new_dir += dir;
			}
			new_dir /= dir_set.size();
			dir_set.pop_back();
			dir_set.push_back(new_dir);
			
			DEBUG << "new dir:" << new_dir << endl;

			// return to the beginning of directions set
			dir_idx = 0;
		}
		
		/* check stopping condition */
		if (
			rho < _config.rho_final
			|| fev > _config.maxNOfEvaluations
		) {
			stop = true;
		}
	}
	
	Vector result = Math::toStdVector(current_guess, dim);
	return result;
}
