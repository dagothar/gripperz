/**
 * @file Rosenbrock.hpp
 * @author Adam Wolniakowski
 * @date 4-07-2015
 */

#pragma once

#include <math/ObjectiveFunction.hpp>

namespace gripperz {
namespace util {

/**
 * @class Rosenbrock
 * @brief Rosenbrock objective function
 */
class Rosenbrock : public math::ObjectiveFunction {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<Rosenbrock> Ptr;

public:
	Rosenbrock(double a=1, double b=100);
	virtual ~Rosenbrock() {};
	
	virtual double evaluate(const std::vector<double>& x);

private:
	double _a, _b;
};

} /* util */
} /* gripperz */
