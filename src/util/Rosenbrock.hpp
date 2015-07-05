/**
 * @file Rosenbrock.hpp
 * @author Adam Wolniakowski
 * @date 4-07-2015
 */

#pragma once

#include <optimization/ObjectiveFunction.hpp>

namespace gripperz {
namespace util {

/**
 * @class Rosenbrock
 * @brief Rosenbrock objective function
 */
class Rosenbrock : public optimization::ObjectiveFunction {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<Rosenbrock> Ptr;

public:
	Rosenbrock(double a, double b);
	virtual ~Rosenbrock() {};
	
	virtual double operator()(const std::vector<double>& x);

private:
	double _a, _b;
};

} /* util */
} /* gripperz */
