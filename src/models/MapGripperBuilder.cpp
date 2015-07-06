/**
 * @file MapGripperBuilder.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "MapGripperBuilder.hpp"


using namespace gripperz::models;
using namespace rw::common;
using namespace rw::math;
using namespace std;


MapGripperBuilder::MapGripperBuilder(Gripper::Ptr prototype, const std::vector<ParameterName>& parameters) :
	_prototype(prototype),
	_parameters(parameters)
{
}


MapGripperBuilder::~MapGripperBuilder() {
}


void MapGripperBuilder::applyParameter(Gripper::Ptr gripper, ParameterName name, double value) {
	Q jawP = gripper->getJawParameters();
	
	switch (name) {
		case Length:
			jawP(1) = value;
			break;
		
		case Width:
			jawP(2) = value;
			break;
		
		case Depth:
			jawP(3) = value;
			break;
		
		case ChfDepth:
			jawP(4) = value;
			break;
		
		case ChfAngle:
			jawP(5) = value * Deg2Rad;
			break;
		
		case CutDepth:
			jawP(7) = value;
			break;
		
		case CutAngle:
			jawP(8) = value * Deg2Rad;
			break;
		
		case CutTilt:
			jawP(10) = value * Deg2Rad;
			break;
		
		case TcpOffset:
			jawP(6) = value;
			gripper->setTCP(Transform3D<>(Vector3D<>(0, 0, value)));
			break;
		
		case Force:
			gripper->setForce(value);
			break;
		
		default:
			RW_WARN("Parameter not implemented!");
	}
	
	gripper->setJawGeometry(jawP);
}


Gripper::Ptr MapGripperBuilder::parametersToGripper(const std::vector<double>& params) {
	if (params.size() != _parameters.size()) {
		RW_THROW("Parameters vector size mismatch!");
	}
	
	Gripper::Ptr gripper = ownedPtr(new Gripper(*_prototype));
	
	for (unsigned i = 0; i < params.size(); ++i) {
		applyParameter(gripper, _parameters[i], params[i]);
	}
	
	return gripper;
}


double MapGripperBuilder::fetchParameter(Gripper::Ptr gripper, ParameterName name) {
	Q jawP = gripper->getJawParameters();
	
	switch (name) {
		case Length:
			return jawP(1);
		
		case Width:
			return jawP(2);
		
		case Depth:
			return jawP(3);
		
		case ChfDepth:
			return jawP(4);
		
		case ChfAngle:
			return jawP(5) * Rad2Deg;
		
		case CutDepth:
			return jawP(7);
		
		case CutAngle:
			return jawP(8) * Rad2Deg;
		
		case CutTilt:
			return jawP(10) * Rad2Deg;
		
		case TcpOffset:
			return jawP(6);
		
		case Force:
			return gripper->getForce();
		
		default:
			RW_WARN("Parameter not implemented!");
	}
	
	RW_THROW("Should never get here!");
	return 0.0;
}


std::vector<double> MapGripperBuilder::gripperToParameters(Gripper::Ptr gripper) {

	vector<double> params;
	
	for (unsigned i = 0; i < _parameters.size(); ++i) {
		params.push_back(fetchParameter(gripper, _parameters[i]));
	}
	
	return params;
}
