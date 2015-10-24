#include "alignment_experiment.hpp"

#include <rw/common/Ptr.hpp>
#include <rw/common/Log.hpp>
#include <rw/math/Math.hpp>
#include <rw/math/Transform3D.hpp>
#include <rw/math/MetricFactory.hpp>
#include <rws/RobWorkStudio.hpp>
#include <rwsim/dynamics/DynamicWorkCell.hpp>
#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>
#include <grasps/filters/ClearStatusFilter.hpp>
#include <grasps/filters/RobustnessGraspFilter.hpp>
#include <grasps/filters/GraspGridFilter.hpp>
#include <evaluation/calculators/AlignmentIndexCalculator.hpp>
#include "RenderTarget.hpp"
#include "grasps/GraspStatistics.hpp"
#include "simulation/AlignmentSimulator.hpp"
#include "SimpleAlignmentMetric.hpp"
#include "grasps/filters/GraspGridFilter.hpp"

using namespace std;
using namespace rws;
using namespace rw::common;
using namespace rw::math;
using namespace rw::models;
using namespace rw::kinematics;
using namespace rwlibs::task;
using namespace rwsim::dynamics;
using namespace gripperz::simulation;
using namespace gripperz::evaluation;
using namespace gripperz::evaluation::calculators;
using namespace gripperz::grasps;
using namespace gripperz::grasps::filters;
using namespace boost;

alignment_experiment::alignment_experiment() :
RobWorkStudioPlugin("simple_sim", QIcon(":/pa_icon.png")) {

    _ui.setupUi(this);
    setupGUI();

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateView()));

    _expectedPose = Transform3D<>(Vector3D<>(0, 0, 0), Rotation3D<>(1, 0, 0, 0, 0, -1, 0, 1, 0));
}

alignment_experiment::~alignment_experiment() {
}

void alignment_experiment::initialize() {
    /* Initialization is basically only adding an event listener to the plugin,
     * so we know when another DynamicWorkCell is loaded.
     */
    getRobWorkStudio()->genericEvent().add(boost::bind(&alignment_experiment::genericEventListener, this, _1), this);

    getRobWorkStudio()->keyEvent().add(boost::bind(&alignment_experiment::keyEventListener, this, _1, _2), this);

    Log::log().setLevel(Log::Info);
}

void alignment_experiment::open(WorkCell * workcell) {
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

void alignment_experiment::close() {
    _timer->stop();
}

void alignment_experiment::setupGUI() {
    connect(_ui.resetButton, SIGNAL(clicked()), this, SLOT(resetState()));
    connect(_ui.loadButton, SIGNAL(clicked()), this, SLOT(loadTasks()));
    connect(_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveTasks()));
    connect(_ui.startButton, SIGNAL(clicked()), this, SLOT(startSimulation()));
    connect(_ui.stopButton, SIGNAL(clicked()), this, SLOT(stopSimulation()));
    connect(_ui.showButton, SIGNAL(clicked()), this, SLOT(showTasks()));
    //connect(_ui.progressBar, SIGNAL(clicked()), this, SLOT(showTasks()));
    connect(_ui.clearButton, SIGNAL(clicked()), this, SLOT(clearStatus()));
    connect(_ui.undoButton, SIGNAL(clicked()), this, SLOT(undoPerturb()));
    connect(_ui.randomPerturbButton, SIGNAL(clicked()), this, SLOT(randomPerturb()));
    connect(_ui.regularPerturbButton, SIGNAL(clicked()), this, SLOT(regularPerturb()));
}

void alignment_experiment::updateView() {
    if (_simulator == NULL || _wc == NULL || _dwc == NULL) {
        return;
    }

    getRobWorkStudio()->setState(_simulator->getSimulators()[0]->getState());

    _ui.progressBar->setValue(_simulator->getNrTasksDone());

    if (!_simulator->isRunning()) {
        _timer->stop();
        showTasks();

        postSimulation();
    }
}

void alignment_experiment::printResults() {
    int all = GraspStatistics::countGraspTargets(_grasps);
    int aligned = GraspStatistics::countGraspsWithStatus(_grasps, GraspResult::Success);
    int misaligned = GraspStatistics::countGraspsWithStatus(_grasps, GraspResult::Interference);
    int successes = aligned + misaligned;
    int failures = all - successes;

    log().info() << "\nExperiment results:\n";
    log().info() << " - grasps = " << all << '\n';
    log().info() << " - aligned = " << aligned << "(" << 100.0 * aligned / all << "%)\n";
    log().info() << " - misaligned = " << misaligned << "(" << 100.0 * misaligned / all << "%)\n";
    log().info() << " - successes(a+m) = " << successes << "(" << 100.0 * successes / all << "%)\n";
    log().info() << " - failures = " << failures << "(" << 100.0 * failures / all << "%)\n" << endl;
}

void alignment_experiment::genericEventListener(const std::string & event) {
    if (event == "DynamicWorkCellLoaded") {

        DynamicWorkCell::Ptr dwc = getRobWorkStudio()->getPropertyMap().get<DynamicWorkCell::Ptr>("DynamicWorkcell", NULL);

        if (dwc == NULL) {
            log().error() << "Could not load dynamic workcell from propertymap!" << std::endl;
            return;
        }

        _dwc = dwc;
    }
}

void alignment_experiment::keyEventListener(int key, Qt::KeyboardModifiers modifier) {
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

void alignment_experiment::resetState() {
    getRobWorkStudio()->setState(_wc->getDefaultState());
}

void alignment_experiment::loadTasks() {
    QString taskfile = QFileDialog::getOpenFileName(this, "Open file", "", tr("Task files (*.xml)"));

    if (taskfile.isEmpty()) {
        return;
    }

    log().info() << "Loading tasks from: " << taskfile.toStdString() << "\n";

    _grasps = GraspTask::load(taskfile.toStdString());

    //GraspFilter::Ptr clearStatusFilter = new ClearStatusFilter();
    //_grasps = clearStatusFilter->filter(_grasps);
    _previousGrasps = _grasps;

    _ui.progressBar->setValue(0);
    _ui.progressBar->setMaximum(_grasps->getAllTargets().size());

    showTasks();
}

void alignment_experiment::saveTasks() {
    QString taskfile = QFileDialog::getSaveFileName(this, "Save file", "", tr("Task files (*.xml)"));

    if (taskfile.isEmpty()) {
        return;
    }

    log().info() << "Saving tasks to: " << taskfile.toStdString() << endl;

    GraspTask::saveRWTask(_grasps, taskfile.toStdString());
}

void alignment_experiment::startSimulation() {
    if (_grasps == NULL) return;

    double threshold = _ui.thresholdLineEdit->text().toDouble() * Deg2Rad;
    AlignmentSimulator::AlignmentMetric::Ptr metric = ownedPtr(new SimpleAlignmentMetric<double>(Vector3D<>(0, 1, 0)));
    //rw::math::MetricFactory::makeTransform3DMetric<double>(0.0, 1.0);

    _simulator = ownedPtr(new AlignmentSimulator(_dwc, _expectedPose, threshold, 1, metric));
    _simulator->loadTasks(_grasps);

    Log::log().setLevel(Log::Info);

    try {
        _simulator->start(_initState);
    } catch (...) {
        return;
    }

    _timer->start();
}

void alignment_experiment::stopSimulation() {
    if (_simulator == NULL) return;

    if (_simulator->isRunning()) {
        _simulator->stop();
    }
}

void alignment_experiment::undoPerturb() {
    _grasps = _previousGrasps;
    showTasks();
}

void alignment_experiment::randomPerturb() {
    if (_grasps == NULL) return;

    _previousGrasps = _grasps;

    int targets = _ui.randomTargetsLineEdit->text().toInt();
    double sigma_p = _ui.randomPositionLineEdit->text().toDouble();
    double sigma_a = _ui.randomAngleLineEdit->text().toDouble();

    try {
        GraspFilter::Ptr robustnessFilter = new RobustnessGraspFilter(targets, sigma_p, sigma_a * Deg2Rad);

        _grasps = robustnessFilter->filter(_grasps);

    } catch (rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }

    _ui.progressBar->setValue(0);
    _ui.progressBar->setMaximum(_grasps->getAllTargets().size());

    showTasks();
}

void alignment_experiment::regularPerturb() {
if (_grasps == NULL) return;

    _previousGrasps = _grasps;

    vector<double> min{
        _ui.xMinEdit->text().toDouble(),
        _ui.yMinEdit->text().toDouble(),
        _ui.zMinEdit->text().toDouble(),
        _ui.rollMinEdit->text().toDouble() * Deg2Rad,
        _ui.pitchMinEdit->text().toDouble() * Deg2Rad,
        _ui.yawMinEdit->text().toDouble() * Deg2Rad
    };
    vector<double> max{
        _ui.xMaxEdit->text().toDouble(),
        _ui.yMaxEdit->text().toDouble(),
        _ui.zMaxEdit->text().toDouble(),
        _ui.rollMaxEdit->text().toDouble() * Deg2Rad,
        _ui.pitchMaxEdit->text().toDouble() * Deg2Rad,
        _ui.yawMaxEdit->text().toDouble() * Deg2Rad
    };
    vector<int> res{
        _ui.xResEdit->text().toInt(),
        _ui.yResEdit->text().toInt(),
        _ui.zResEdit->text().toInt(),
        _ui.rollResEdit->text().toInt(),
        _ui.pitchResEdit->text().toInt(),
        _ui.yawResEdit->text().toInt(),
    };

    try {
        GraspFilter::Ptr regularFilter = new GraspGridFilter(min, max, res);

        _grasps = regularFilter->filter(_grasps);

    } catch (rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }

    _ui.progressBar->setValue(0);
    _ui.progressBar->setMaximum(_grasps->getAllTargets().size());

    showTasks();
}

void alignment_experiment::showTasks() {
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

void alignment_experiment::clearStatus() {
    if (_grasps == NULL) return;

    try {
        GraspFilter::Ptr filter = new ClearStatusFilter();

        _grasps = filter->filter(_grasps);

    } catch (rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }

    showTasks();
}

void alignment_experiment::postSimulation() {
    printResults();

    QualityIndexCalculator::Ptr calculator = new AlignmentIndexCalculator(_ui.filteringLineEdit->text().toDouble());
    double alignment_index = calculator->calculate(NULL, _grasps);

    log().info() << "Alignment Index = " << alignment_index << endl;
}



Q_EXPORT_PLUGIN(alignment_experiment);
