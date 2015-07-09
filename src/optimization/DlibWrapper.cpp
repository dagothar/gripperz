/**
 * @file DlibWrapper.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#include "DlibWrapper.hpp"
#include "DlibUtil.hpp"


using namespace std;
using namespace gripperz::optimization;


DlibWrapper::DlibWrapper(ObjectiveFunction::Ptr func) :
	_objFunc(func)
{}


DlibWrapper::~DlibWrapper() {}


double DlibWrapper::operator()(const dlib::matrix<double, 0, 1>& x) {
	vector<double> params = DlibUtil::dlibToVector(x);
	
	double y = _objFunc->operator()(params);
	
	return y;
}
