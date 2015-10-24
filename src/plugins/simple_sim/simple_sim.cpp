#include "simple_sim.hpp"

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
using namespace rwsim::dynamics;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace boost;

simple_sim::simple_sim() :
RobWorkStudioPlugin("simple_sim", QIcon(":/pa_icon.png")) {

    _ui.setupUi(this);
    setupGUI();

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateView()));
}

simple_sim::~simple_sim() {
}

void simple_sim::initialize() {
    /* Initialization is basically only adding an event listener to the plugin,
     * so we know when another DynamicWorkCell is loaded.
     */
    getRobWorkStudio()->genericEvent().add(
                                           boost::bind(&simple_sim::genericEventListener, this, _1), this);

    getRobWorkStudio()->keyEvent().add(
                                       boost::bind(&simple_sim::keyEventListener, this, _1, _2), this);

    Log::log().setLevel(Log::Info);
}

//void simple_sim::startSimulation() {
//
//    _simulator = ownedPtr(new BasicSimulator(_dwc, 1));
//    //_simulator->loadTasks(_grasps);
//
//    Log::log().setLevel(Log::Info);
//
//    try {
//        _simulator->start(_initState);
//    } catch (...) {
//        return;
//    }
//
//    _timer->start();
//}

void simple_sim::open(WorkCell * workcell) {
    try {
        Math::seed(TimerUtil::currentTimeUs());

        _wc = workcell;

        _initState = getRobWorkStudio()->getState();

    } catch (const rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }
}

void simple_sim::close() {
    _timer->stop();
}

void simple_sim::setupGUI() {
    //    connect(ui.editSetupButton, SIGNAL(clicked()), this, SLOT(setupEvent()));
    //    connect(ui.loadSetupButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.saveSetupButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.addHintButton, SIGNAL(clicked()), this, SLOT(addHint()));
    //    connect(ui.clearHintsButton, SIGNAL(clicked()), this, SLOT(clearHints()));
    //    connect(ui.designButton, SIGNAL(clicked()), this, SLOT(designEvent()));
    //    connect(ui.loadGripperButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.saveGripperButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.gripperCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(guiEvent(int)));
    //    connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.initialButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.loadTaskButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.saveTaskButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.planButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.nAutoEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
    //    connect(ui.showCheck, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.samplesCheck, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.successCheck, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.startButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.testButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
    //    connect(ui.perturbButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
}

void simple_sim::updateView() {
    if (_simulator == NULL || _wc == NULL || _dwc == NULL) {
        return;
    }

    getRobWorkStudio()->setState(_simulator->getSimulators()[0]->getState());

    // check out the number of tasks already performed and update progress bar accordingly
    _ui.progressBar->setValue(_simulator->getNrTasksDone());
}

void simple_sim::genericEventListener(const std::string & event) {
    if (event == "DynamicWorkCellLoaded") {

        DynamicWorkCell::Ptr dwc = getRobWorkStudio()->getPropertyMap().get<DynamicWorkCell::Ptr>("DynamicWorkcell", NULL);

        if (dwc == NULL) {
            log().error() << "Could not load dynamic workcell from propertymap!" << std::endl;
            return;
        }

        _dwc = dwc;
    }
}

void simple_sim::keyEventListener(int key, Qt::KeyboardModifiers modifier) {
    /* let's move the view */
    Transform3D<> viewT = getRobWorkStudio()->getViewTransform();
    Rotation3D<> rot;
    Transform3D<> nviewT;

    switch (key) {
        case 'A':
            rot = EAA<>(Vector3D<>::z(), -5.0 * Deg2Rad).toRotation3D();
            nviewT = Transform3D<>(rot * viewT.P(), rot * viewT.R());
            getRobWorkStudio()->setViewTransform(nviewT);
            getRobWorkStudio()->updateAndRepaint();
            break;

        case 'D':
            rot = EAA<>(Vector3D<>::z(), 5.0 * Deg2Rad).toRotation3D();
            nviewT = Transform3D<>(rot * viewT.P(), rot * viewT.R());
            getRobWorkStudio()->setViewTransform(nviewT);
            getRobWorkStudio()->updateAndRepaint();
            break;

        case 'W':
            rot = EAA<>(viewT.R() * Vector3D<>::x(), -5.0 * Deg2Rad).toRotation3D();
            nviewT = Transform3D<>(rot * viewT.P(), rot * viewT.R());
            getRobWorkStudio()->setViewTransform(nviewT);
            getRobWorkStudio()->updateAndRepaint();
            break;

        case 'S':
            rot = EAA<>(viewT.R() * Vector3D<>::x(), 5.0 * Deg2Rad).toRotation3D();
            nviewT = Transform3D<>(rot * viewT.P(), rot * viewT.R());
            getRobWorkStudio()->setViewTransform(nviewT);
            getRobWorkStudio()->updateAndRepaint();
            break;

        case 'Z':
            nviewT = Transform3D<>(1.1 * viewT.P(), viewT.R());
            getRobWorkStudio()->setViewTransform(nviewT);
            getRobWorkStudio()->updateAndRepaint();
            break;

        case 'X':
            nviewT = Transform3D<>(viewT.P() / 1.1, viewT.R());
            getRobWorkStudio()->setViewTransform(nviewT);
            getRobWorkStudio()->updateAndRepaint();
            break;
    }
}

Q_EXPORT_PLUGIN(simple_sim);
