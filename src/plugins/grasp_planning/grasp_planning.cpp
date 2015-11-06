#include "grasp_planning.hpp"
#include "geometry/OpenSCADGenerator.hpp"
#include "models/loaders/StandardGripperLoader.hpp"

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
using namespace gripperz::parametrization;
using namespace gripperz::models;
using namespace gripperz::models::loaders;
using namespace gripperz::geometry;

grasp_planning::grasp_planning() :
RobWorkStudioPlugin("grasp_planning", QIcon(":/planning_icon.png")) {

    _ui.setupUi(this);
}

grasp_planning::~grasp_planning() {
}

void grasp_planning::initialize() {
    /* Initialization is basically only adding an event listener to the plugin,
     * so we know when another DynamicWorkCell is loaded.
     */
    getRobWorkStudio()->genericEvent().add(boost::bind(&grasp_planning::genericEventListener, this, _1), this);

    Log::log().setLevel(Log::Info);
}

void grasp_planning::open(WorkCell * workcell) {
    try {
        Math::seed(TimerUtil::currentTimeUs());

        _wc = workcell;
        _state = _wc->getDefaultState();

    } catch (const rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }
}

void grasp_planning::close() {
}

//void grasp_planning::setupGUI() {
////    connect(_ui.loadButton, SIGNAL(clicked()), this, SLOT(loadGripper()));
////    connect(_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveGripper()));
////    //connect(_ui.updateButton, SIGNAL(clicked()), this, SLOT(updateGripper()));
////    connect(_ui.applyButton, SIGNAL(clicked()), this, SLOT(applyGripper()));
//}

void grasp_planning::genericEventListener(const std::string & event) {
    if (event == "DynamicWorkCellLoaded") {

        DynamicWorkCell::Ptr dwc = getRobWorkStudio()->getPropertyMap().get<DynamicWorkCell::Ptr>("DynamicWorkcell", NULL);

        if (dwc == NULL) {
            log().error() << "Could not load dynamic workcell from propertymap!" << std::endl;
            return;
        }

        _dwc = dwc;
    }
}

Q_EXPORT_PLUGIN(grasp_planning);
