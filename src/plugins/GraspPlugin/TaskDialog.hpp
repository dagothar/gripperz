/**
 * @file TaskDialog.hpp
 * @author Adam Wolniakowski
 */

#pragma once

#include <QDialog>
#include <context/TaskDescription.hpp>
#include "ui_TaskDialog.h"

class QLineEdit;
class QPushButton;
class QRadioButton;
class QComboBox;

/**
 * @class TaskDialog
 * @brief Dialog for editing TaskDescription in gripper evaluation plugin..
 */
class TaskDialog: public QDialog {
	Q_OBJECT

public:
	// constructors
	/// Constructor
	TaskDialog(
		QWidget* parent = 0,
		gripperz::context::TaskDescription::Ptr td = 0,
		std::string wd = ""
	);

	/// Destructor
	virtual ~TaskDialog() {
	}

	// methods

private slots:
	void guiEvent();
	void guiEvent(int index);

private:
	// methods
	//void createGUI();
	void updateGUI();
	void updateTaskDescription();

	// data
	gripperz::context::TaskDescription::Ptr _td;
	bool _changed;

	// GUI
	Ui::taskDialog ui;

	std::string _wd;
};

