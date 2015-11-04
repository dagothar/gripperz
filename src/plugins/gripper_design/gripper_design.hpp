#pragma once

#include <QObject>
#include <rwlibs/task.hpp>
#include <rws/RobWorkStudioPlugin.hpp>
#include <rw/models/WorkCell.hpp>
#include <rw/kinematics/State.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include <QtGui>
#include <QTimer>
#include "models/StandardGripper.hpp"

#include "ui_gripper_design.h"

/**
 * @brief A plugin for testing grippers.
 */
class gripper_design: public rws::RobWorkStudioPlugin {
	Q_OBJECT
	Q_INTERFACES(rws::RobWorkStudioPlugin)

public:
	//! @brief constructor
	gripper_design();

	//! @brief destructor
	virtual ~gripper_design();

	//! @copydoc rws::RobWorkStudioPlugin::open(rw::models::WorkCell* workcell)
	virtual void open(rw::models::WorkCell* workcell);

	//! @copydoc rws::RobWorkStudioPlugin::close()
	virtual void close();

	//! @copydoc rws::RobWorkStudioPlugin::initialize()
	virtual void initialize();

	/**
	 * @brief we listen for events regarding opening and closing of dynamic
	 * workcell
	 * @param event [in] the event id
	 */
	void genericEventListener(const std::string& event);

private slots:
        void makeGripper();
        void loadGripper();
        void saveGripper();
        void applyGripper();
        void updateGripper();
        void updateGUI();
        void updateParameterTable();
        void updateParametrization();

private:
	void setupGUI();
        gripperz::parametrization::Parametrization::Ptr makeParametrization();

	rw::models::WorkCell* _wc;
	rwsim::dynamics::DynamicWorkCell::Ptr _dwc;
        rw::kinematics::State _state;
        gripperz::models::StandardGripper::Ptr _gripper;

	Ui::GripperDesignWidget _ui;
};
