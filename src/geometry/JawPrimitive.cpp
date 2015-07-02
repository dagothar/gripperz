#include "JawPrimitive.hpp"

#include <sstream>
#include <rw/common/Ptr.hpp>
#include <rw/geometry/PlainTriMesh.hpp>
#include <rwlibs/csg/CSGModel.hpp>
#include <rwlibs/csg/CSGModelFactory.hpp>



using namespace std;
using namespace rw::geometry;
using namespace rw::math;
using namespace rw::common;
using namespace rwlibs::csg;



JawPrimitive::JawPrimitive(const rw::math::Q& initQ)
{
	if(!(initQ.size() == 10 || initQ.size() == 11)) {
		RW_THROW("Size of parameter list must equal 10 or 11 (cut tilt)!");
	}
	
	int i = 0;
	_type = CutoutType(initQ(i++));
	_length = initQ(i++);
	_width = initQ(i++);
	_depth = initQ(i++);
	_chamferDepth = initQ(i++);
	_chamferAngle = initQ(i++);
	_cutPosition = initQ(i++);
	_cutDepth = initQ(i++);
	_cutAngle = initQ(i++);
	_cutRadius = initQ(i++);
	
	if (initQ.size() == 11) {
		_cutTilt = initQ(i++);
	}
}



JawPrimitive::~JawPrimitive()
{
}



TriMesh::Ptr JawPrimitive::createMesh(int resolution) const
{
	//use CSG
	CSGModel::Ptr base = CSGModelFactory::makeBox(_length, _depth, _width);
	base->translate(_length/2, 0, _width/2);
	
	// make chamfer
	Vector3D<> point(_length, 0.0, (1-_chamferDepth)*_width);
	Transform3D<> t(Vector3D<>(), RPY<>(0, -_chamferAngle, 0).toRotation3D());
	Vector3D<> normal = t * Vector3D<>::x();
	CSGModel::Ptr plane = CSGModelFactory::makePlane(point, -normal);
	base->subtract(plane);
	
	// make cutout
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
	
	TriMesh::Ptr mesh = base->getTriMesh();

	return mesh;
}

rw::math::Q JawPrimitive::getParameters() const
{
	Q q(11);
	
	int i = 0;
	q(i++) = _type;
	q(i++) = _length;
	q(i++) = _width;
	q(i++) = _depth;
	q(i++) = _chamferDepth;
	q(i++) = _chamferAngle;
	q(i++) = _cutPosition;
	q(i++) = _cutDepth;
	q(i++) = _cutAngle;
	q(i++) = _cutRadius;
	q(i++) = _cutTilt;
	
	return q;
}


void JawPrimitive::setParameters(const rw::math::Q& q) {
	int i = 0;
	_type = (int)q(i++) == 0 ? Prismatic : Cylindrical;
	_length = q(i++);
	_width = q(i++);
	_depth = q(i++);
	_chamferDepth = q(i++);
	_chamferAngle = q(i++);
	_cutPosition = q(i++);
	_cutDepth = q(i++);
	_cutAngle = q(i++);
	_cutRadius = q(i++);
	_cutTilt = q(i++);
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
