/**
 * @file Rosenbrock.hpp
 * @author Adam Wolniakowski
 * @date 4-07-2015
 */

#include "Rosenbrock.hpp"


using namespace gripperz::util;
using namespace gripperz::math;


Rosenbrock::Rosenbrock(double a, double b) :
	_a(a),
	_b(b)
{}


double Rosenbrock::evaluate(const std::vector<double>& p) {
	double x = p[0];
	double y = p[1];
	
	double ax = _a - x;
	double ax2 = ax * ax;
	
	double yx2 = y - x*x;
	double byx22 = _b * yx2 * yx2;
	
	double result = ax2 + byx22;
	
	return result;
}
