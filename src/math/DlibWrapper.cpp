/**
 * @file DlibWrapper.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#include "DlibWrapper.hpp"
#include <util/DlibUtil.hpp>


using namespace std;
using namespace gripperz::math;
using namespace gripperz::util;


DlibWrapper::DlibWrapper(ObjectiveFunction::Ptr func) :
	_objFunc(func)
{}


DlibWrapper::~DlibWrapper() {}


Scalar DlibWrapper::evaluate(const dlib::matrix<double, 0, 1>& x) {
	Vector params = DlibUtil::dlibToVector(x);
	
	Scalar y = _objFunc->evaluate(params);
	
	return y;
}
