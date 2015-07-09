/**
 * @file DlibUtil.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-09
 */

#include "DlibUtil.hpp"


using namespace gripperz::optimization;
using namespace std;


dlib::matrix<double, 0, 1> DlibUtil::vectorToDlib(const std::vector<double>& vec) {
	dlib::matrix<double, 0, 1> res;
	
	for (unsigned i = 0; i < vec.size(); ++i) {
		res(i) = vec[i];
	}
	
	return res;
}
	

std::vector<double> DlibUtil::dlibToVector(const dlib::matrix<double, 0, 1>& vec) {
	vector<double> params(vec.nr());
	
	for (unsigned i = 0; i < params.size(); ++i) {
		params[i] = vec(i);
	}
	
	return params;
}
