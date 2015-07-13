#include "DesignDialog.hpp"

#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <QtGui>
#include <QRadioButton>

#include <loaders/GripperXMLLoader.hpp>

using namespace std;
USE_ROBWORK_NAMESPACE
;
using namespace robwork;
using namespace rwsim;
using namespace gripperz::loaders;
using namespace gripperz::models;

DesignDialog::DesignDialog(QWidget* parent, Gripper::Ptr gripper,
		std::string wd) :
		QDialog(parent), _gripper(gripper), _changed(false), _wd(wd) {
	if (!_gripper)
		_gripper = ownedPtr(new Gripper);

	ui.setupUi(this);

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.applyButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.defaultButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	
	connect(ui.nameEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	
	connect(ui.lengthEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.widthEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.depthEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.chfDepthEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.chfAngleEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.cutDepthEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.cutAngleEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.cutTiltEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.tcpEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.jawdistEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.strokeEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.forceEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.basexEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.baseyEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.basezEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	
	connect(ui.successEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.robustnessEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.alignmentEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.coverageEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.wrenchEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.topWrenchEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.stressEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.volumeEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.qualityEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));

	_updateGUI();
}

void DesignDialog::guiEvent() {
	QObject* obj = sender();

	if (obj == ui.okButton) {
		_gripper = ownedPtr(new Gripper);
		_updateGripper();
		close();
	}

	else if (obj == ui.applyButton) {
		_updateGripper();
	}

	else if (obj == ui.defaultButton) {
		_gripper = ownedPtr(new Gripper);
		_updateGUI();
	}

	_changed = true;
}

void DesignDialog::_updateGripper() {
	if (_gripper) {
		/* update parameters */
		_gripper->setLength(ui.lengthEdit->text().toDouble());
		_gripper->setWidth(ui.widthEdit->text().toDouble());
		_gripper->setDepth(ui.depthEdit->text().toDouble());
		_gripper->setChfDepth(ui.chfDepthEdit->text().toDouble());
		_gripper->setChfAngle(ui.chfAngleEdit->text().toDouble());
		_gripper->setCutDepth(ui.cutDepthEdit->text().toDouble());
		_gripper->setCutAngle(ui.cutAngleEdit->text().toDouble());
		_gripper->setCutTilt(ui.cutTiltEdit->text().toDouble());
		_gripper->setTcpOffset(ui.tcpEdit->text().toDouble());
		_gripper->setJawdist(ui.jawdistEdit->text().toDouble());
		_gripper->setStroke(ui.strokeEdit->text().toDouble());
		_gripper->setForce(ui.forceEdit->text().toDouble());
		_gripper->setBaseX(ui.basexEdit->text().toDouble());
		_gripper->setBaseY(ui.baseyEdit->text().toDouble());
		_gripper->setBaseZ(ui.basezEdit->text().toDouble());

		/* update results */
		_gripper->getQuality().success = ui.successEdit->text().toDouble();
		_gripper->getQuality().robustness = ui.robustnessEdit->text().toDouble();
		_gripper->getQuality().alignment = ui.alignmentEdit->text().toDouble();
		_gripper->getQuality().coverage = ui.coverageEdit->text().toDouble();
		_gripper->getQuality().wrench = ui.wrenchEdit->text().toDouble();
		_gripper->getQuality().topwrench = ui.topWrenchEdit->text().toDouble();
		_gripper->getQuality().stress = ui.stressEdit->text().toDouble();
		_gripper->getQuality().volume = ui.volumeEdit->text().toDouble();
		_gripper->getQuality().quality = ui.qualityEdit->text().toDouble();
	}
}

void DesignDialog::_updateGUI() {
	if (_gripper) {
		/* update parameters */
		ui.lengthEdit->setText(QString::number(_gripper->getLength()));
		ui.widthEdit->setText(QString::number(_gripper->getWidth()));
		ui.depthEdit->setText(QString::number(_gripper->getDepth()));
		ui.chfDepthEdit->setText(QString::number(_gripper->getChfDepth()));
		ui.chfAngleEdit->setText(QString::number(_gripper->getChfAngle()));
		ui.cutDepthEdit->setText(QString::number(_gripper->getCutDepth()));
		ui.cutAngleEdit->setText(QString::number(_gripper->getCutAngle()));
		ui.cutTiltEdit->setText(QString::number(_gripper->getCutTilt()));
		ui.nameEdit->setText(QString::fromStdString(_gripper->getName()));
		ui.tcpEdit->setText(QString::number(_gripper->getTcpOffset()));
		ui.forceEdit->setText(QString::number(_gripper->getForce()));
		ui.jawdistEdit->setText(QString::number(_gripper->getJawdist()));
		ui.strokeEdit->setText(QString::number(_gripper->getStroke()));
		ui.basexEdit->setText(QString::number(_gripper->getBaseX()));
		ui.baseyEdit->setText(QString::number(_gripper->getBaseY()));
		ui.basezEdit->setText(QString::number(_gripper->getBaseZ()));

		/* update results area */
		ui.coverageEdit->setText(QString::number(_gripper->getQuality().coverage));
		ui.successEdit->setText(QString::number(_gripper->getQuality().success));
		ui.robustnessEdit->setText(QString::number(_gripper->getQuality().robustness));
		ui.alignmentEdit->setText(QString::number(_gripper->getQuality().alignment));
		ui.wrenchEdit->setText(QString::number(_gripper->getQuality().wrench));
		ui.topWrenchEdit->setText(QString::number(_gripper->getQuality().topwrench));
		ui.stressEdit->setText(QString::number(_gripper->getQuality().stress));
		ui.volumeEdit->setText(QString::number(_gripper->getQuality().volume));
		ui.qualityEdit->setText(QString::number(_gripper->getQuality().quality));
	}
}
