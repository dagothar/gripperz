#include "gripper_design.hpp"

#include <rw/common/Ptr.hpp>
#include <rw/common/Log.hpp>
#include <rw/math/Math.hpp>
#include <rw/math/Transform3D.hpp>
#include <rws/RobWorkStudio.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

using namespace std;
using namespace rws;
using namespace rw::common;
using namespace rw::math;
using namespace rw::models;
using namespace rw::kinematics;
using namespace rwlibs::task;
using namespace rwsim::dynamics;
using namespace boost;

gripper_design::gripper_design() :
RobWorkStudioPlugin("gripper_design", QIcon(":/pa_icon.png")) {

    _ui.setupUi(this);
    setupGUI();
}

gripper_design::~gripper_design() {
}

void gripper_design::initialize() {
    /* Initialization is basically only adding an event listener to the plugin,
     * so we know when another DynamicWorkCell is loaded.
     */
    getRobWorkStudio()->genericEvent().add(boost::bind(&gripper_design::genericEventListener, this, _1), this);

    Log::log().setLevel(Log::Info);
}

void gripper_design::open(WorkCell * workcell) {
    try {
        Math::seed(TimerUtil::currentTimeUs());

        _wc = workcell;

    } catch (const rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }
}

void gripper_design::close() {
}

void gripper_design::setupGUI() {
//    connect(_ui.resetButton, SIGNAL(clicked()), this, SLOT(resetState()));
//    connect(_ui.loadButton, SIGNAL(clicked()), this, SLOT(loadTasks()));
//    connect(_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveTasks()));
//    connect(_ui.startButton, SIGNAL(clicked()), this, SLOT(startSimulation()));
//    connect(_ui.stopButton, SIGNAL(clicked()), this, SLOT(stopSimulation()));
//    connect(_ui.showButton, SIGNAL(clicked()), this, SLOT(showTasks()));
}

void gripper_design::updateView() {
//    getRobWorkStudio()->setState(_simulator->getSimulators()[0]->getState());
}

void gripper_design::genericEventListener(const std::string & event) {
    if (event == "DynamicWorkCellLoaded") {

        DynamicWorkCell::Ptr dwc = getRobWorkStudio()->getPropertyMap().get<DynamicWorkCell::Ptr>("DynamicWorkcell", NULL);

        if (dwc == NULL) {
            log().error() << "Could not load dynamic workcell from propertymap!" << std::endl;
            return;
        }

        _dwc = dwc;
    }
}

Q_EXPORT_PLUGIN(gripper_design);
