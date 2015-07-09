/**
 * @file DlibWrapper.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#include "DlibWrapper.hpp"


using namespace std;
using namespace gripperz::optimization;


DlibWrapper::DlibWrapper(ObjectiveFunction::Ptr func) :
	_objFunc(func)
{}


DlibWrapper::~DlibWrapper() {}


double DlibWrapper::operator()(const dlib::matrix<double, 0, 1>& x) {
	vector<double> params(x.nr());
	
	for (unsigned i = 0; i < params.size(); ++i) {
		params[i] = x(i);
	}
	
	double y = _objFunc->operator()(params);
	
	return y;
}
