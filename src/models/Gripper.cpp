#include "Gripper.hpp"

#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>


#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
using namespace rwsim;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::geometry;



Gripper::Gripper(const std::string& name) :
	_name(name),
	_length(0.1),
	_width(0.025),
	_depth(0.02),
	_chfdepth(0),
	_chfangle(0),
	_cutdepth(0),
	_cutangle(0),
	_cuttilt(0),
	_tcpoffset(0.025),
	_jawdist(0),
	_stroke(0.05),
	_force(10),
	_basex(0.15),
	_basey(0.1),
	_basez(0.05)
{
}


rw::geometry::Geometry::Ptr Gripper::getFingerGeometry() const {
	Q q(11);
	
	q(0) = 0;
	q(1) = _length;
	q(2) = _width;
	q(3) = _depth;
	q(4) = _chfdepth;
	q(5) = _chfangle * Deg2Rad;
	q(6) = _length - _tcpoffset;
	q(7) = _cutdepth;
	q(8) = _cutangle * Deg2Rad;
	q(9) = 0.0;
	q(10) = _cuttilt * Deg2Rad;
	
	Geometry::Ptr fingerGeo = ownedPtr(new Geometry(new JawPrimitive(q), std::string("FingerGeo")));
	
	return fingerGeo;
}


rw::geometry::Geometry::Ptr Gripper::getBaseGeometry() const {
	Q q(3, _basex, _basey, _basez);
	
	Geometry::Ptr baseGeo = ownedPtr(new Geometry(new Box(q), std::string("BaseGeo")));
	
	return baseGeo;
}


void Gripper::updateGripper(
	rw::models::WorkCell::Ptr wc,
	rwsim::dynamics::DynamicWorkCell::Ptr dwc,
	rw::models::TreeDevice::Ptr dev,
	rwsim::dynamics::RigidDevice::Ptr ddev,
	rw::kinematics::State& state,
	TaskDescription::Ptr td
) {
	Geometry::Ptr baseGeometry = getBaseGeometry();
	Geometry::Ptr leftGeometry = getFingerGeometry();
	Geometry::Ptr rightGeometry = getFingerGeometry();

	// remove existing objects
	DEBUG << "- Removing objects..." << endl;
	wc->removeObject(wc->findObject("gripper.Base").get());
	wc->removeObject(wc->findObject("gripper.LeftFinger").get());
	wc->removeObject(wc->findObject("gripper.RightFinger").get());
	DEBUG << "- Objects removed." << endl;

	// create and add new objects
	DEBUG << "- Adding new objects..." << endl;

	// if base is parametrized, the box has to be moved from origin by half its height
	Transform3D<> baseT;
	baseT = Transform3D<>(-0.5 * _basez * Vector3D<>::z());

	RigidObject* baseobj = new RigidObject(wc->findFrame("gripper.Base"));
	Model3D* basemodel = new Model3D("BaseModel");
	basemodel->addTriMesh(Model3D::Material("stlmat", 0.4f, 0.4f, 0.4f),
			*baseGeometry->getGeometryData()->getTriMesh());
	basemodel->setTransform(baseT);
	baseGeometry->setTransform(baseT);
	baseobj->addModel(basemodel);
	baseobj->addGeometry(baseGeometry);
	wc->add(baseobj);
	dwc->findBody("gripper.Base")->setObject(baseobj);

	RigidObject* leftobj = new RigidObject(wc->findFrame("gripper.LeftFinger"));
	Model3D* leftmodel = new Model3D("LeftModel");
	leftmodel->addTriMesh(Model3D::Material("stlmat", 0.4f, 0.4f, 0.4f),
			*leftGeometry->getGeometryData()->getTriMesh());
	leftmodel->setTransform(Transform3D<>());
	leftGeometry->setTransform(Transform3D<>());
	leftobj->addModel(leftmodel);
	leftobj->addGeometry(leftGeometry);
	wc->add(leftobj);
	dwc->findBody("gripper.LeftFinger")->setObject(leftobj);

	RigidObject* rightobj = new RigidObject(wc->findFrame("gripper.RightFinger"));
	Model3D* rightmodel = new Model3D("RightModel");
	rightmodel->addTriMesh(Model3D::Material("stlmat", 0.4f, 0.4f, 0.4f),
			*rightGeometry->getGeometryData()->getTriMesh());
	rightmodel->setTransform(
			Transform3D<>(Vector3D<>(),
					Rotation3D<>(1, 0, 0, 0, 1, 0, 0, 0, -1)));
	rightGeometry->setTransform(
			Transform3D<>(Vector3D<>(),
					Rotation3D<>(1, 0, 0, 0, 1, 0, 0, 0, -1)));
	rightobj->addModel(rightmodel);
	rightobj->addGeometry(rightGeometry);
	wc->add(rightobj);
	dwc->findBody("gripper.RightFinger")->setObject(rightobj);
	DEBUG << "Objects added." << endl;

	// set tcp
	MovableFrame* tcp = (MovableFrame*) td->getGripperTCP();
	tcp->setTransform(Transform3D<>(Vector3D<>(0, 0, _length - _tcpoffset)), state);

	// set bounds
	double minOpening = 0.5*_jawdist;

	dev->setBounds(make_pair(Q(1, minOpening), Q(1, minOpening + 0.5*_stroke)));
	dev->setQ(Q(1, minOpening), state);

	// set force
	ddev->setMotorForceLimits(Q(2, _force, _force));

	DEBUG << "Gripper updated!" << endl;
}


double Gripper::getCrossHeight(double x) const {

	if (x > _length)
		return 0.0; // far beyond the gripper

	double lwidth = _width;

	// check if to subtract from the width due to the chamfering
	double d = _length - _chfdepth * _width * tan(Deg2Rad * _chfangle);
	if (x > d) {
		lwidth = _width - (x - d) * 1.0 / tan(Deg2Rad * _chfangle);
	}

	// check if subtract from the width due to the cut
	double cutpos = _length - _tcpoffset;
	double cutdist = abs(x - cutpos);

	if (cutdist < _cutdepth * tan(Deg2Rad * _cutangle / 2.0)) {
		lwidth -= _cutdepth - cutdist * tan(1.57 - Deg2Rad * _cutangle / 2.0);
	}

	if (lwidth < 0.0) {
		lwidth = 0.0;
	}

	return lwidth;
}


double Gripper::getMaxStress() const {
	double sigmaMax = 0.0;

	for (double x = 0.001; x < _length; x += 0.001) {
		
		double h = 100 * getCrossHeight(x);
		double b = 100 * _depth;
		double M = (_length - x) * _force;
		double sigma = 6 * M / (b * h * h);
		if (std::isinf(sigma)) {
			sigma = 1000000.0;
		}
		if (std::isnan(sigma)) {
			sigma = 0.0;
		}

		if (sigma > sigmaMax) {
			sigmaMax = sigma;
		}
	}

	return sigmaMax;
}


double Gripper::getVolume() const {
	double volume = 1e6 * GeometryUtil::estimateVolume(*getFingerGeometry()->getGeometryData()->getTriMesh());
	
	return volume;
}
