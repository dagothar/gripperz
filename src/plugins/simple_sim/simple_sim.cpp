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
#include <grasps/filters/ClearStatusFilter.hpp>
#include "RenderTarget.hpp"

using namespace std;
using namespace rws;
using namespace rw::common;
using namespace rw::math;
using namespace rw::models;
using namespace rw::kinematics;
using namespace rwlibs::task;
using namespace rwsim::dynamics;
using namespace gripperz::simulation;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
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
    getRobWorkStudio()->genericEvent().add(boost::bind(&simple_sim::genericEventListener, this, _1), this);

    getRobWorkStudio()->keyEvent().add(boost::bind(&simple_sim::keyEventListener, this, _1, _2), this);

    Log::log().setLevel(Log::Info);
}

void simple_sim::open(WorkCell * workcell) {
    try {
        Math::seed(TimerUtil::currentTimeUs());

        _wc = workcell;

        _initState = getRobWorkStudio()->getState();

        _render = ownedPtr(new RenderTargets());
        getRobWorkStudio()->getWorkCellScene()->addRender("pointRender", _render, workcell->getWorldFrame());

    } catch (const rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }
}

void simple_sim::close() {
    _timer->stop();
}

void simple_sim::setupGUI() {
    connect(_ui.resetButton, SIGNAL(clicked()), this, SLOT(resetState()));
    connect(_ui.loadButton, SIGNAL(clicked()), this, SLOT(loadTasks()));
    connect(_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveTasks()));
    connect(_ui.startButton, SIGNAL(clicked()), this, SLOT(startSimulation()));
    connect(_ui.stopButton, SIGNAL(clicked()), this, SLOT(stopSimulation()));
    connect(_ui.showButton, SIGNAL(clicked()), this, SLOT(showTasks()));
}

void simple_sim::updateView() {
    if (_simulator == NULL || _wc == NULL || _dwc == NULL) {
        return;
    }

    getRobWorkStudio()->setState(_simulator->getSimulators()[0]->getState());

    _ui.progressBar->setValue(_simulator->getNrTasksDone());

    if (!_simulator->isRunning()) {
        _timer->stop();
    }
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

void simple_sim::resetState() {
    getRobWorkStudio()->setState(_wc->getDefaultState());
}

void simple_sim::loadTasks() {
    QString taskfile = QFileDialog::getOpenFileName(this, "Open file", "", tr("Task files (*.xml)"));

    if (taskfile.isEmpty()) {
        return;
    }

    log().info() << "Loading tasks from: " << taskfile.toStdString() << "\n";

    _grasps = GraspTask::load(taskfile.toStdString());

    GraspFilter::Ptr clearStatusFilter = new ClearStatusFilter();
    _grasps = clearStatusFilter->filter(_grasps);

    _ui.progressBar->setValue(0);
    _ui.progressBar->setMaximum(_grasps->getAllTargets().size());

    showTasks();
}

void simple_sim::saveTasks() {
    QString taskfile = QFileDialog::getSaveFileName(this, "Save file", "", tr("Task files (*.xml)"));

    if (taskfile.isEmpty()) {
        return;
    }

    log().info() << "Saving tasks to: " << taskfile.toStdString() << endl;

    GraspTask::saveRWTask(_grasps, taskfile.toStdString());
}

void simple_sim::startSimulation() {

    _simulator = ownedPtr(new BasicSimulator(_dwc, 1));
    _simulator->loadTasks(_grasps);

    Log::log().setLevel(Log::Info);

    try {
        _simulator->start(_initState);
    } catch (...) {
        return;
    }

    _timer->start();
}

void simple_sim::stopSimulation() {
    if (_simulator->isRunning()) {
        _simulator->stop();
    }
}

void simple_sim::showTasks() {
    vector<RenderTargets::Target> rtargets;
    Transform3D<> wTo = Kinematics::worldTframe(_wc->findFrame(_ui.targetEdit->text().toStdString()), _wc->getDefaultState());

    rwlibs::task::GraspTask::Ptr tasks = _grasps;
    if (tasks == NULL) {
        return;
    }

    typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(TaskTarget p, tasks->getAllTargets()) {
        RenderTargets::Target rt;
        rt.ctask = p.first;
        rt.ctarget = *p.second; //target;

        if (rt.ctarget.getResult()->testStatus == GraspResult::UnInitialized) {
            rt.color[0] = 0.0;
            rt.color[1] = 0.9;
            rt.color[2] = 1.0;
            rt.color[3] = 0.5;
        } else if (rt.ctarget.getResult()->testStatus == GraspResult::Success) {
            rt.color[0] = 0.0;
            rt.color[1] = 1.0;
            rt.color[2] = 0.0;
            rt.color[3] = 0.5;
        } else if (rt.ctarget.getResult()->testStatus == GraspResult::Interference) {
            rt.color[0] = 1.0;
            rt.color[1] = 1.0;
            rt.color[2] = 0.0;
            rt.color[3] = 0.5;
        } else {
            rt.color[0] = 1.0;
            rt.color[1] = 0.0;
            rt.color[2] = 0.0;
            rt.color[3] = 0.5;
        }

        rt.trans = wTo * rt.ctarget.pose;

        rtargets.push_back(rt);
    }


    ((RenderTargets*) _render.get())->setTargets(rtargets);
    getRobWorkStudio()->postUpdateAndRepaint();
}

Q_EXPORT_PLUGIN(simple_sim);
