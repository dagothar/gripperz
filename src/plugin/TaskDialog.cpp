#include "TaskDialog.hpp"

#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <QtGui>
#include <QRadioButton>
#include <QComboBox>


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
using namespace rwsim;
using namespace gripperz::context;


TaskDialog::TaskDialog(QWidget* parent, TaskDescription::Ptr td, std::string wd) :
		QDialog(parent), _td(td), _changed(false), _wd(wd) {
	//createGUI();
	ui.setupUi(this);

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.applyButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));

	updateGUI();
}

void TaskDialog::guiEvent() {
	QObject* obj = sender();

	if (obj == ui.okButton) {
		updateTaskDescription();
		close();
	}

	else if (obj == ui.applyButton) {
		updateTaskDescription();
	}
}

void TaskDialog::guiEvent(int index) {
	QObject* obj = sender();

	if (obj == ui.gripperCombo) {
		// we have to find proper frames and update GUI

	}
}

void TaskDialog::updateGUI() {
	// fill the target box
	ui.targetCombo->clear();
	BOOST_FOREACH (Object::Ptr obj, _td->getWorkCell()->getObjects()) {
		ui.targetCombo->addItem(QString::fromStdString(obj->getName()));
	}
	ui.targetCombo->setCurrentIndex(
			ui.targetCombo->findText(
					QString::fromStdString(_td->getTargetObject()->getName())));

	// fill the gripper box
	ui.gripperCombo->clear();
	BOOST_FOREACH (Device::Ptr dev, _td->getWorkCell()->getDevices()) {
		ui.gripperCombo->addItem(QString::fromStdString(dev->getName()));
	}
	ui.gripperCombo->setCurrentIndex(
			ui.gripperCombo->findText(
					QString::fromStdString(
							_td->getGripperDevice()->getName())));

	// update the distances section
	Q teachDist = _td->getTeachDistance();
	teachDist(3) *= Rad2Deg;
	teachDist(4) *= Rad2Deg;
	stringstream ss;
	ss << teachDist;
	ui.teachDistEdit->setText(QString::fromStdString(ss.str()));

	Q preDist = _td->getPrefilteringDistance();
	preDist(1) *= Rad2Deg;
	preDist(2) *= Rad2Deg;
	ss.str("");
	ss << preDist;
	ui.preDistEdit->setText(QString::fromStdString(ss.str()));

	Q covDist = _td->getCoverageDistance();
	covDist(1) *= Rad2Deg;
	covDist(2) *= Rad2Deg;
	ss.str("");
	ss << covDist;
	ui.coverageDistEdit->setText(QString::fromStdString(ss.str()));

	// update the limits section
	ui.intLimitEdit->setText(QString::number(_td->getInterferenceLimit()));
	ui.wreLimitEdit->setText(QString::number(_td->getWrenchLimit()));
}

void TaskDialog::updateTaskDescription() {
	// set new target
	_td->setTargetObject(ui.targetCombo->currentText().toStdString());

	// update distances
	Q teachDist;
	stringstream ss(ui.teachDistEdit->text().toStdString());
	ss >> teachDist;
	teachDist(3) *= Deg2Rad;
	teachDist(4) *= Deg2Rad;
	_td->setTeachDistance(teachDist);

	Q preDist;
	ss.str(ui.preDistEdit->text().toStdString());
	ss >> preDist;
	//covDist(6) *= Deg2Rad;
	preDist(1) *= Deg2Rad;
	preDist(2) *= Deg2Rad;
	_td->setPrefilteringDistance(preDist);

	Q covDist;
	ss.str(ui.coverageDistEdit->text().toStdString());
	ss >> covDist;
	//covDist(6) *= Deg2Rad;
	covDist(1) *= Deg2Rad;
	covDist(2) *= Deg2Rad;
	_td->setCoverageDistance(covDist);

	// update limits
	_td->setInterferenceLimit(ui.intLimitEdit->text().toDouble());
	_td->setWrenchLimit(ui.wreLimitEdit->text().toDouble());
}
