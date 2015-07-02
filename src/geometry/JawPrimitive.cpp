#include "JawPrimitive.hpp"

#include <sstream>
#include <rw/common/Ptr.hpp>
#include <rw/geometry/PlainTriMesh.hpp>
#include <rwlibs/csg/CSGModel.hpp>
#include <rwlibs/csg/CSGModelFactory.hpp>


using namespace gripperz::geometry;

using namespace std;
using namespace rw::geometry;
using namespace rw::math;
using namespace rw::common;
using namespace rwlibs::csg;



JawPrimitive::JawPrimitive(const rw::math::Q& initQ)
{
	setParameters(initQ);
}


JawPrimitive::~JawPrimitive()
{
}


TriMesh::Ptr JawPrimitive::createMesh(int resolution) const
{
	/* build base */
	CSGModel::Ptr base = CSGModelFactory::makeBox(_length, _depth, _width);
	base->translate(_length/2, 0, _width/2);
	
	/* make chamfer */
	Vector3D<> point(_length, 0.0, (1-_chamferDepth)*_width);
	Transform3D<> t(Vector3D<>(), RPY<>(0, -_chamferAngle, 0).toRotation3D());
	Vector3D<> normal = t * Vector3D<>::x();
	CSGModel::Ptr plane = CSGModelFactory::makePlane(point, -normal);
	base->subtract(plane);
	
	/* make cutout */
	if (_type == Cylindrical) {
		CSGModel::Ptr cutout = CSGModelFactory::makeCylinder(_cutRadius, 100.0);
		cutout->rotate(-90*Deg2Rad, 90*Deg2Rad, 0);
		cutout->rotate(_cutTilt, 0, 0);
		cutout->translate(_cutPosition, 0, _cutDepth-_cutRadius);
		base->subtract(cutout);
	} else {
		CSGModel::Ptr cutout = CSGModelFactory::makeWedge(_cutAngle);
		cutout->rotate(-90*Deg2Rad, 90*Deg2Rad, 0);
		cutout->rotate(_cutTilt, 0, 0);
		cutout->translate(_cutPosition, 0, _cutDepth);
		base->subtract(cutout);
	}
	
	/* generate mesh */
	TriMesh::Ptr mesh = base->getTriMesh();

	return mesh;
}


rw::math::Q JawPrimitive::getParameters() const
{
	Q q(11);
	
	q(0) = _type;
	q(1) = _length;
	q(2) = _width;
	q(3) = _depth;
	q(4) = _chamferDepth;
	q(5) = _chamferAngle;
	q(6) = _cutPosition;
	q(7) = _cutDepth;
	q(8) = _cutAngle;
	q(9) = _cutRadius;
	q(10) = _cutTilt;
	
	return q;
}


void JawPrimitive::setParameters(const rw::math::Q& q) {
	if(!(q.size() == 11)) {
		RW_THROW("initQ must be of size 11!");
	}
	
	_type 			= CutoutType(q(0));
	_length			= q(1);
	_width 			= q(2);
	_depth 			= q(3);
	_chamferDepth 	= q(4);
	_chamferAngle 	= q(5);
	_cutPosition 	= q(6);
	_cutDepth 		= q(7);
	_cutAngle 		= q(8);
	_cutRadius 		= q(9);
	_cutTilt 		= q(10);
}


std::string JawPrimitive::toString() const
{
	stringstream str;
	
	str << _type << " " << _length << " " << _width << " " << _depth << " " << _chamferDepth << " " << Rad2Deg*_chamferAngle;
	str << " " << _cutPosition << " " << _cutDepth << " ";
	if (_type == Cylindrical) {
		str << _cutRadius;
	} else {
		str << Rad2Deg*_cutAngle;
	}
	str << " " << Rad2Deg*_cutTilt;
	
	return str.str();
}
