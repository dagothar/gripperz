/**
 * @file SimulatedAnnealingOptimizer.cpp
 * @author Adam Wolniakowski
 * @date 2015-09-06
 */

#include "SimulatedAnnealingOptimizer.hpp"
#include <boost/foreach.hpp>
#include <rw/math/Q.hpp>
#include <rw/math/Math.hpp>


#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::optimization;
using namespace gripperz::math;
using namespace std;
using namespace rw::math;


SimulatedAnnealingOptimizer::SimulatedAnnealingOptimizer(const Configuration& config) :
	_config(config)
{
}


SimulatedAnnealingOptimizer::~SimulatedAnnealingOptimizer() {
}


Vector generateNewState(const Vector& currentState, Scalar epsilon) {
	unsigned dim = currentState.size();
	
	Q q(dim);
	q = Math::fromStdVector(currentState, q);
	q += Math::ranDir(dim, Math::ran(0.0, epsilon));
	
	Vector newState = Math::toStdVector(q, dim);
	
	return newState;
}


double SimulatedAnnealingOptimizer::calculateAcceptanceProbability(double e0, double e1, double T) {
	if (e1 < e0) {
		return 1.0;
	} else if (T == 0.0) {
		return 0.0;
	} else {
		return exp(-fabs(e0 - e1) / T);
	}
}


Vector SimulatedAnnealingOptimizer::minimize(ObjectiveFunction::Ptr function, const Vector& initialGuess) {
	
	double dT = ( _config.rho_0 - _config.rho_final ) / _config.maxNOfEvaluations;
	double T = _config.rho_0;
	unsigned fev = 0;
	
	Vector current_state = initialGuess;
	Scalar current_value = function->evaluate(current_state); ++fev;
	
	do {
		Vector new_state = generateNewState(current_state, T);
		Scalar new_value = function->evaluate(new_state); ++fev;
		
		if (rw::math::Math::ran(0.0, 1.0) < calculateAcceptanceProbability(current_value, new_value, T)) {
			current_state = new_state;
			current_value = new_value;
		}
		
		T -= dT; if (T < 0.0) T = 0.0;
		
	} while (T > _config.rho_final && fev < _config.maxNOfEvaluations);
	
	return current_state;
}
