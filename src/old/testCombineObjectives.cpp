/**
 * @file testCombineObjectives.hpp
 * @author Adam Wolniakowski
 * @date 01-07-2015
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <math/CombineObjectivesFactory.hpp>


using namespace std;
using namespace gripperz::math;


vector<double> readVector(istream& stream) {
	vector<double> v;
	
	string str;
	getline(stream, str);
	stringstream sstr(str);
	
	double d;
	while (sstr >> d) {
		v.push_back(d);
	}
	
	return v;
}


string vectorToString(const vector<double>& v) {
	stringstream sstr;
	
	for (unsigned i = 0; i < v.size(); ++i) {
		sstr << v[i] << " ";
	}
	
	return sstr.str();
}


int main(int argc, char* argv[]) {
	/* read weights */
	cout << "Please input weights: ";
	vector<double> weights = readVector(cin);
	cout << "Weights: " << vectorToString(weights) << endl;
	
	/* initialize combiners */
	CombineObjectives::Ptr sumMethod = CombineObjectivesFactory::make("sum", weights);
	CombineObjectives::Ptr productMethod = CombineObjectivesFactory::make("product", weights);
	CombineObjectives::Ptr logMethod = CombineObjectivesFactory::make("log", weights);
	
	/* try a number of objectives */
	while (true) {
		cout << "Please input objectives: ";
		vector<double> obj = readVector(cin);
		
		cout << "Sum=" << sumMethod->combine(obj)
			<< " Product=" << productMethod->combine(obj)
			<< " Log=" << logMethod->combine(obj) << endl;
	}
	
	return 0;
}
