#pragma once

#include <QObject>
#include <rwlibs/task.hpp>
#include <rws/RobWorkStudioPlugin.hpp>
#include <rw/models/WorkCell.hpp>
#include <rw/kinematics/State.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include <QtGui>
#include <QTimer>

#include "ui_grasp_planning.h"

/**
 * @brief A plugin for testing grippers.
 */
class grasp_planning: public rws::RobWorkStudioPlugin {
	Q_OBJECT
	Q_INTERFACES(rws::RobWorkStudioPlugin)

public:
	//! @brief constructor
	grasp_planning();

	//! @brief destructor
	virtual ~grasp_planning();

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

private:
	rw::models::WorkCell* _wc;
	rwsim::dynamics::DynamicWorkCell::Ptr _dwc;
        rw::kinematics::State _state;

	Ui::GripperDesignWidget _ui;
};
