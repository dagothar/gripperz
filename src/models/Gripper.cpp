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
		_name(name), _baseGeometry(NULL), _leftGeometry(NULL), _rightGeometry(
				NULL), _tcp(Transform3D<>(Vector3D<>(0, 0, 0.075))), _jawdist(
				0), _opening(0.05), _force(50) {
	setBaseGeometry(Q(3, 0.15, 0.1, 0.05));
	setJawGeometry(Q(10, 0, 0.1, 0.025, 0.02, 0, 0, 0.05, 0, 90 * Deg2Rad, 0));
}

void Gripper::updateGripper(rw::models::WorkCell::Ptr wc,
		rwsim::dynamics::DynamicWorkCell::Ptr dwc,
		rw::models::TreeDevice::Ptr dev, rwsim::dynamics::RigidDevice::Ptr ddev,
		rw::kinematics::State& state, TaskDescription::Ptr td) {
	Geometry::Ptr baseGeometry = getBaseGeometry();
	Geometry::Ptr leftGeometry = getJawGeometry();
	Geometry::Ptr rightGeometry = getJawGeometry();

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
	if (_isBaseParametrized) {
		baseT = Transform3D<>(-0.5 * _baseParameters(2) * Vector3D<>::z());
	}

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
	tcp->setTransform(_tcp, state);

	// set bounds
	double minOpening = _jawdist;
	if (minOpening < 0.0)
		minOpening = 0.0;

	dev->setBounds(make_pair(Q(1, minOpening), Q(1, _opening)));
	dev->setQ(Q(1, minOpening), state);

	// set force
	ddev->setMotorForceLimits(Q(2, _force, _force));

	DEBUG << "Gripper updated!" << endl;
}

double Gripper::getCrossHeight(double x) const {
	if (_isJawParametrized) { // jaw is parametrized -- easy

		double length = _jawParameters(1);

		if (x > length)
			return 0.0; // far beyond the gripper

		//double depth = _jawParameters(3);
		double width = _jawParameters(2);
		double lwidth = width;

		// check if to subtract from the width due to the chamfering
		double chfDepth = _jawParameters(4);
		double chfAngle = _jawParameters(5);
		double d = length - chfDepth * width * tan(chfAngle);
		if (x > d) {
			lwidth = width - (x - d) * 1.0 / tan(chfAngle);
		}

		// check if subtract from the width due to the cut
		double cutPos = _jawParameters(6);
		double cutDepth = _jawParameters(7);
		double cutAngle = _jawParameters(8);
		double cutDist = abs(x - cutPos);

		if (cutDist < cutDepth * tan(cutAngle / 2.0)) {
			lwidth -= cutDepth - cutDist * tan(1.57 - cutAngle / 2.0);
		}

		if (lwidth < 0.0) {
			lwidth = 0.0;
		}

		return lwidth;

	} else {
		// TODO: calculate stl's crossection somehow

		return 0.0;
	}
}

double Gripper::getMaxStress() const {
	if (!_isJawParametrized) {
		return 0.0; // TODO: add calculations for STL
	}

	double length = _jawParameters(1);

	double sigmaMax = 0.0;

	for (double x = 0.0; x < length; x += 0.001) {
		double h = 100 * getCrossHeight(x);
		double b = 100 * _jawParameters(3);
		double M = x > length ? 0.0 : (length - x) * _force;
		double sigma = 6 * M / (b * h * h);
		if (isinf(sigma)) {
			sigma = 1000000.0;
		}
		if (isnan(sigma)) {
			sigma = 0.0;
		}

		if (sigma > sigmaMax) {
			sigmaMax = sigma;
		}
	}

	return sigmaMax;
}

double Gripper::getVolume() const {
	return _jawParameters(1) * _jawParameters(2) * _jawParameters(3);
}
