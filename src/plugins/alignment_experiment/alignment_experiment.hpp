#pragma once

#include <QObject>
#include <rwlibs/task.hpp>
#include <rws/RobWorkStudioPlugin.hpp>
#include <rw/models/WorkCell.hpp>
#include <rw/kinematics/State.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include <simulation/BasicSimulator.hpp>
#include <grasps/Types.hpp>
#include <QtGui>
#include <QTimer>

#include "ui_alignment_experiment.h"

/**
 * @brief A plugin for testing grippers.
 */
class alignment_experiment: public rws::RobWorkStudioPlugin {
	Q_OBJECT
	Q_INTERFACES(rws::RobWorkStudioPlugin)

public:
	//! @brief constructor
	alignment_experiment();

	//! @brief destructor
	virtual ~alignment_experiment();

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

	/// listens for key presses
	void keyEventListener(int key, Qt::KeyboardModifiers modifier);

private slots:
	//! Updates RWS state according to the simulation
	void updateView();
        
        void resetState();
        void loadTasks();
        void saveTasks();
        void startSimulation();
        void stopSimulation();
        void showTasks();
        void clearStatus();
        void undoPerturb();
        void randomPerturb();
        void regularPerturb();

private:
	void setupGUI();
        void printResults();
        void postSimulation();

	rw::models::WorkCell* _wc;
	rwsim::dynamics::DynamicWorkCell::Ptr _dwc;
	gripperz::simulation::BasicSimulator::Ptr _simulator;
	rw::kinematics::State _initState;
        gripperz::grasps::Grasps _grasps;
        gripperz::grasps::Grasps _previousGrasps;
        rw::math::Transform3D<> _expectedPose;

	rw::graphics::Render::Ptr _render; // used to render targets
	QTimer *_timer; // used to update RWS view periodically

	Ui::AlignmentExpWidget _ui;
};
