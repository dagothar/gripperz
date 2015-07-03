/**
 * @file testDlibRosenbrock.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#include <iostream>
#include <dlib/optimization.h>


using namespace std;


int fev = 0;


double rosenbrock(const dlib::matrix<double, 0, 1>& vec) {
	const double a = 1;
	const double b = 100;
	
	double x = vec(0);
	double y = vec(1);
	
	double ax = a - x;
	double ax2 = ax * ax;
	
	double yx2 = y - x*x;
	double byx22 = b * yx2 * yx2;
	
	double result = ax2 + byx22;
	
	cout << "rosenbrock(" << x << ", " << y << ")=" << result << endl;
	++fev;
	
	return result;
}


int main(int argc, char* argv[]) {
	/* starting point */
	dlib::matrix<double, 0, 1> initialGuess(2);
	dlib::matrix<double, 0, 1> lowerBound(2);
	dlib::matrix<double, 0, 1> upperBound(2);
	
	initialGuess = 0, 0;
	lowerBound = -2, -2;
	upperBound = 2, 2;
	
	/* perform actual optimization */
	try {
		dlib::find_min_bobyqa<
			double(const dlib::matrix<double, 0, 1>&),
			dlib::matrix<double, 0, 1>,
			dlib::matrix<double, 0, 1>
		>(
			rosenbrock, // function to minimize
			initialGuess,
			initialGuess.size() * 2 + 1, // number of interpolation points
			lowerBound,
			upperBound,
			0.2, // initial trust region radius
			1e-6, // stopping trust region radius
			1000 // max number of f evaluations
		);
	} catch (dlib::bobyqa_failure& e) {
		cout << "Exception: " << e.info << endl;
	}
	
	dlib::matrix<double, 0, 1>& result = initialGuess;
	cout << "Result: x=" << result(0) << " y=" << result(1) << " f=" << rosenbrock(result) << endl;
	cout << "Number of function evaluations: " << fev << endl;
	
	return 0;
}
