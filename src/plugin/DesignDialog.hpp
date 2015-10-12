/**
 * @file DesignDialog.hpp
 * @author Adam Wolniakowski
 */

#pragma once

#include <QDialog>
#include <models/OldGripper.hpp>
#include "ui_DesignDialog.h"

class QLineEdit;
class QPushButton;
class QRadioButton;

/**
 * @class DesignDialog
 * @brief Dialog for gripper design in the plugin.
 */
class DesignDialog: public QDialog {
	Q_OBJECT

public:
	// constructors
	/// Constructor
	DesignDialog(
		QWidget* parent = 0,
		gripperz::models::OldGripper::Ptr gripper = 0,
		std::string wd = ""
	);

	/// Destructor
	virtual ~DesignDialog() {
	}

	// methods
	gripperz::models::OldGripper::Ptr getGripper() {
		return _gripper;
	}
	std::string getWorkingDirectory() {
		return _wd;
	}
	bool isChanged() const {
		return _changed;
	}

private slots:
	void guiEvent();

private:
	// methods
	void _createGUI();
	void _updateGUI();
	void _updateGripper();

	// data
	gripperz::models::OldGripper::Ptr _gripper;
	bool _changed;

	// GUI
	Ui::designDialog ui;

	std::string _wd;
};
