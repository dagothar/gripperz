#include "GraspPlugin.hpp"

#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>
#include <rws/RobWorkStudio.hpp>
#include <rw/graspplanning/GWSMeasure3D.hpp>
#include <rwlibs/opengl/Drawable.hpp>
#include <rwsim/util/SurfacePoseSampler.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <rwsimlibs/ode/ODESimulator.hpp>
#include <rw/proximity/CollisionDetector.hpp>
#include <rw/common/Exception.hpp>
#include <boost/lexical_cast.hpp>
#include <rwlibs/algorithms/kdtree/KDTree.hpp>
#include <rwlibs/algorithms/kdtree/KDTreeQ.hpp>
#include <fstream>
#include <iostream>

#include <geometry/JawPrimitive.hpp>
#include "RenderTarget.hpp"
#include <grasps/TaskGenerator.hpp>
#include "DesignDialog.hpp"
#include <simulator/GripperTaskSimulator.hpp>
#include <loaders/GripperXMLLoader.hpp>
#include <loaders/TaskDescriptionLoader.hpp>
#include "TaskDialog.hpp"


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
using namespace robworksim;
using namespace rws;
using namespace rwlibs::proximitystrategies;
using namespace rwlibs::simulation;
using namespace rwlibs::task;
using namespace gripperz::models;
using namespace gripperz::context;
using namespace gripperz::loaders;
using namespace gripperz::grasps;
using namespace gripperz::simulator;
using namespace gripperz::evaluation;


GraspPlugin::GraspPlugin() :
		RobWorkStudioPlugin("GraspPlugin", QIcon(":/pa_icon.png")), _wc(NULL), _dwc(
				NULL), _simulator(NULL), _slowMotion(false), _showTasks(true), _showSamples(
				false), _showSuccesses(false), _silentMode(false), _nOfTargetsToGen(
				10), _tasks(NULL), _td(NULL), _wd(""), _interferenceLimit(0.1), _wrenchLimit(
				0.0) {
	ui.setupUi(this);
	setupGUI();

	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateSim()));

	_gripper = ownedPtr(new Gripper);
}

GraspPlugin::~GraspPlugin() {
}

void GraspPlugin::initialize() {
	/* Initialization is basically only adding an event listener to the plugin,
	 * so we know when another DynamicWorkCell is loaded.
	 */
	getRobWorkStudio()->genericEvent().add(
			boost::bind(&GraspPlugin::genericEventListener, this, _1), this);

	getRobWorkStudio()->keyEvent().add(
			boost::bind(&GraspPlugin::keyEventListener, this, _1, _2), this);
			
	Log::log().setLevel(Log::Info);
}

void GraspPlugin::startSimulation() {

	_simulator = ownedPtr(
		new InterferenceSimulator(_dwc, _td->getInterferenceLimit(), _td->getInterferenceObjects())
	);
	_simulator->loadTasks(_tasks);

	try {
		_td->getInitState().upgrade();
		_simulator->start(_td->getInitState());
	} catch (...) {
		return;
	}

	_timer->start();
}

void GraspPlugin::open(WorkCell* workcell) {
	try {
		Math::seed(TimerUtil::currentTimeUs());

		_wc = workcell;
		_wd = _wc->getFilePath();
		if (!_td && _wc->getPropertyMap().has("taskDescription")) {
			string filename = _wd
					+ _wc->getPropertyMap().get<string>("taskDescription");
			log().info() << "Loading task description from: " << filename
					<< endl;
			cout << "Loading task description from: " << filename << endl;
			_td = TaskDescriptionLoader::load(filename, _dwc);
			cout << "Loaded task description." << endl;
		}

		_initState = getRobWorkStudio()->getState();

		_render = ownedPtr(new RenderTargets());
		getRobWorkStudio()->getWorkCellScene()->addRender("pointRender", _render, workcell->getWorldFrame());
	} catch (const rw::common::Exception& e) {
		QMessageBox::critical(NULL, "RW Exception", e.what());
	}
}

void GraspPlugin::close() {
	_timer->stop();
}

void GraspPlugin::addHint() {
	Transform3D<> target = Kinematics::worldTframe(_td->getGripperTCP(),
			getRobWorkStudio()->getState());
	log().info() << "Added hint grasp: " << target.P() << " "
			<< RPY<>(target.R()) << endl;

	if (_td) {
		_td->addHint(target);
	}
}

void GraspPlugin::clearHints() {
	_td->getHints().clear();
}

void GraspPlugin::setupGUI() {
	connect(ui.editSetupButton, SIGNAL(clicked()), this, SLOT(setupEvent()));
	connect(ui.loadSetupButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.saveSetupButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.addHintButton, SIGNAL(clicked()), this, SLOT(addHint()));
	connect(ui.clearHintsButton, SIGNAL(clicked()), this, SLOT(clearHints()));
	connect(ui.designButton, SIGNAL(clicked()), this, SLOT(designEvent()));
	connect(ui.loadGripperButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.saveGripperButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.gripperCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(guiEvent(int)));
	connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.initialButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.loadTaskButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.saveTaskButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.planButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.nAutoEdit, SIGNAL(editingFinished()), this, SLOT(guiEvent()));
	connect(ui.showCheck, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.samplesCheck, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.successCheck, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.testButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
	connect(ui.perturbButton, SIGNAL(clicked()), this, SLOT(guiEvent()));
}

void GraspPlugin::guiEvent(int i) {
	_gripper = _gripperList[i];

	updateGripper();
}

void GraspPlugin::guiEvent() {
	QObject *obj = sender();

	if (obj == ui.startButton) {
		startSimulation();
	}

	else if (obj == ui.stopButton) {
		if (_simulator->isRunning()) {
			_simulator->stop();
		}
	}

	else if (obj == ui.loadTaskButton) {
		QString taskfile = QFileDialog::getOpenFileName(this, "Open file", "",
				tr("Task files (*.xml)"));

		if (taskfile.isEmpty())
			return;

		log().info() << "Loading tasks from: " << taskfile.toStdString()
				<< "\n";

		_tasks = GraspTask::load(taskfile.toStdString());
		ui.progressBar->setValue(0);
		ui.progressBar->setMaximum(_tasks->getAllTargets().size());

		showTasks();
	}

	else if (obj == ui.saveTaskButton) {
		QString taskfile = QFileDialog::getSaveFileName(this, "Save file", "",
				tr("Task files (*.xml)"));

		if (taskfile.isEmpty())
			return;

		log().info() << "Saving tasks to: " << taskfile.toStdString() << endl;

		GraspTask::saveRWTask(_tasks, taskfile.toStdString());
	}

	else if (obj == ui.initialButton) { // return the workcell to the initial state
		getRobWorkStudio()->setState(_td->getInitState());
	}

	else if (obj == ui.planButton) {
		planTasks();
		showTasks();
	}

	else if (obj == ui.perturbButton) {
		perturbTasks();
		showTasks();
	}

	else if (obj == ui.showCheck) {
		_showTasks = ui.showCheck->isChecked();
		showTasks();
	}

	else if (obj == ui.samplesCheck) {
		_showSamples = ui.samplesCheck->isChecked();
		showTasks();
	}

	else if (obj == ui.successCheck) {
		_showSuccesses = ui.successCheck->isChecked();
		showTasks();
	}

	else if (obj == ui.loadGripperButton) {
		QStringList files = QFileDialog::getOpenFileNames(this, "Open file",
				QString::fromStdString(_wd), tr("Gripper files (*.grp.xml)"));
		if (files.isEmpty())
			return;

		ui.gripperCombo->disconnect(this);
		QStringList f = files;
		QStringList::Iterator it = f.begin();
		while (it != f.end()) {
			cout << "Loading: " << it->toStdString() << endl;
			loadGripper(it->toStdString());
			_wd = QFileInfo(*it).path().toStdString();
			++it;
		}
		connect(ui.gripperCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(guiEvent(int)));
		ui.gripperCombo->setCurrentIndex(0);
	}

	else if (obj == ui.saveGripperButton) {
		QString filename = QFileDialog::getSaveFileName(this, "Save file",
				QString::fromStdString(_wd), tr("Gripper files (*.grp.xml)"));

		if (filename.isEmpty())
			return;

		QFileInfo file(filename);
		if (file.suffix().isEmpty())
			filename += ".grp.xml";
		_wd = file.path().toStdString();

		GripperXMLLoader::save(_gripper, filename.toStdString());
	}

	else if (obj == ui.clearButton) {
		_gripperList.clear();
		ui.gripperCombo->disconnect(this);
		cout << "Number of items in combo box: " << ui.gripperCombo->count()
				<< endl;

		ui.gripperCombo->clear();
	connect(ui.gripperCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(guiEvent(int)));
	}

	else if (obj == ui.loadSetupButton) {
		if (!_dwc) {
			RW_WARN("You first have to open proper dynamic workcell!");
			return;
		}

		QString filename = QFileDialog::getOpenFileName(this, "Open file",
				QString::fromStdString(_wd),
				tr("Task description files (*.td.xml)"));

		if (filename.isEmpty())
			return;

		_wd = QFileInfo(filename).path().toStdString();

		_td = TaskDescriptionLoader::load(filename.toStdString(), _dwc);
	}

	else if (obj == ui.saveSetupButton) {
		QString filename = QFileDialog::getSaveFileName(this, "Open file",
				QString::fromStdString(_wd),
				tr("Task description files (*.td.xml)"));

		if (filename.isEmpty())
			return;

		_wd = QFileInfo(filename).path().toStdString();

		TaskDescriptionLoader::save(_td, filename.toStdString());
	}

	else if (obj == ui.testButton) {
		test();
	}
}

void GraspPlugin::designEvent() {
	cout << "Design dialog opened..." << endl;
	DesignDialog* ddialog = new DesignDialog(this, _gripper, _wd);
	ddialog->exec();

	_wd = ddialog->getWorkingDirectory();

	_gripper = ddialog->getGripper();

	if (ddialog->isChanged()) {
		_gripperList.insert(_gripperList.begin(), _gripper);
		ui.gripperCombo->addItem(QString::fromStdString(_gripper->getName()));
	}

	updateGripper();

	ui.progressBar->setValue(0);
	if (_tasks) {
		ui.progressBar->setMaximum(_tasks->getAllTargets().size());
	} else {
		ui.progressBar->setMaximum(0);
	}
}

void GraspPlugin::setupEvent() {
TaskDialog* sdialog = new TaskDialog(this, _td, _wd);
sdialog->exec();
}

void GraspPlugin::loadGripper(const std::string& filename) {
	cout << "Loading gripper from: " << filename << endl;
	Gripper::Ptr gripper = GripperXMLLoader::load(filename);
	_gripperList.push_back(gripper);
	ui.gripperCombo->addItem(QString::fromStdString(gripper->getName()));
}

void GraspPlugin::updateGripper() {
	cout << "Updating gripper..." << endl;
	_gripper->updateGripper(_td->getWorkCell(), _td->getDynamicWorkCell(),
			_td->getGripperDevice(), _td->getGripperDynamicDevice(),
			_td->getInitState(), _td);

	cout << "Refreshing RWS..." << endl;
	getRobWorkStudio()->getWorkCellScene()->clearCache();
	getRobWorkStudio()->getWorkCellScene()->updateSceneGraph(_td->getInitState());
	getRobWorkStudio()->setWorkcell(_wc);

	getRobWorkStudio()->setState(_td->getInitState());
}

void GraspPlugin::updateSim() {
	if (_simulator == NULL || _wc == NULL || _dwc == NULL)
		return;

	if (!_silentMode && _simulator->isRunning()) {
		getRobWorkStudio()->setState(_simulator->getSimulators()[0]->getState());
	}

	// check out the number of tasks already performed and update progress bar accordingly
	ui.progressBar->setValue(_simulator->getNrTasksDone());

	if (!_simulator->isRunning()) {
		_timer->stop();

		//_gripper->getQuality() = GripperQuality(_graspSim->getGripperQuality());
		GripperEvaluator::Ptr evaluator = ownedPtr(new GripperEvaluator(_td));
		GripperQuality::Ptr quality = evaluator->evaluateGripper(_gripper, _tasks, _samples);
		_gripper->setQuality(*quality);
		
		cout << _gripper->getQuality() << endl;

		log().info() << "Simulation is finished." << endl;
	}
}

GraspTask::Ptr GraspPlugin::generateTasks(int nTasks) {
	GraspTask::Ptr task = ownedPtr(new GraspTask());

	task->setTCPID(_wc->getPropertyMap().get<string>("gripperTCP"));
	task->setGripperID(_wc->getPropertyMap().get<string>("gripper"));
	task->setGraspControllerID(_wc->getPropertyMap().get<string>("controller"));

	task->getSubTasks().resize(1);
	GraspSubTask &subtask = task->getSubTasks()[0];

	subtask.openQ = Q(1, 0.0);
	subtask.closeQ = Q(1, 0.05);

	subtask.approach = inverse(_wTapproach) * _wTtarget;
	subtask.retract = inverse(_wTapproach) * _wTtarget;

	for (int i = 0; i < nTasks; ++i) {
		subtask.targets.push_back(GraspTarget(_wTtarget));
	}

	return task;
}

void GraspPlugin::planTasks() {
	_nOfTargetsToGen = ui.nAutoEdit->text().toInt();

	cout << "Planning " << _nOfTargetsToGen << " tasks..." << endl;
	log().info() << "Generating " << _nOfTargetsToGen << " tasks..." << endl;

	if (_gripper == NULL) {
		cout << "NULL gripper" << endl;
	}

	try {
		_generator = ownedPtr(new TaskGenerator(_td));
		
		/*
		 * Test TCP position
		 */
		cout << "For generation: " << _td->getGripperTCP()->getTransform(_initState) << endl;
		cout << "Updated gripper: " << _td->getGripperTCP()->getTransform(_td->getInitState()) << endl;

		_generator->generateTask(_nOfTargetsToGen, _td->getInitState());

		_tasks = _generator->getTasks();
		_samples = _generator->getSamples();
	} catch (rw::common::Exception& e) {
		QMessageBox::critical(NULL, "RW Exception", e.what());
		
	}
	
	log().info() << "Tasks: " << _tasks->getAllTargets().size()
			<< endl;
	log().info() << "Samples: " << _samples->getAllTargets().size()
			<< endl;
	cout << "Tasks: " << _tasks->getSubTasks()[0].getTargets().size() << endl;
	cout << "Samples: " << _samples->getSubTasks()[0].getTargets().size() << endl;

	cout << "Done." << endl;
	log().info() << "Done." << endl;

	ui.progressBar->setValue(0);
	ui.progressBar->setMaximum(_nOfTargetsToGen);

	showTasks();
}

void GraspPlugin::perturbTasks() {
	bool onlySuccesses = ui.perturbBox->isChecked();
	int nPerturbations = ui.perturbEdit->text().toInt();

	cout << "Perturbing tasks..." << endl;
	log().info() << "Perturbing tasks..." << endl;

	if (_gripper == NULL) {
		cout << "NULL gripper" << endl;
	}

	try {
		if (onlySuccesses)
			_tasks = TaskGenerator::copyTasks(_tasks, true);
		_tasks = TaskGenerator::addPerturbations(_tasks, 0.003, 8.0 * Deg2Rad,
				nPerturbations);

	} catch (rw::common::Exception& e) {

		QMessageBox::critical(NULL, "RW Exception", e.what());
	}

	cout << "Done." << endl;
	log().info() << "Done." << endl;

	ui.progressBar->setValue(0);
	ui.progressBar->setMaximum(nPerturbations);
}

void GraspPlugin::genericEventListener(const std::string& event) {
	if (event == "DynamicWorkCellLoaded") {

		DynamicWorkCell::Ptr dwc = getRobWorkStudio()->getPropertyMap().get<
				DynamicWorkCell::Ptr>("DynamicWorkcell", NULL);

		if (dwc == NULL) {
			log().error() << "Could not load dynamic workcell from propertymap!"
					<< std::endl;
			return;
		}

		_dwc = dwc;

		ui.startButton->setEnabled(true);
	}
}

void GraspPlugin::keyEventListener(int key, Qt::KeyboardModifiers modifier) {
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

void GraspPlugin::showTasks() {
	//return;
	vector<RenderTargets::Target> rtargets;
	Transform3D<> wTo = Kinematics::worldTframe(_td->getTargetFrame(),
			_wc->getDefaultState());

	rwlibs::task::GraspTask::Ptr tasks = NULL;

	if (_showTasks)
		tasks = _tasks;
	if (_showSamples)
		tasks = _samples;
	//if (tasks == NULL) { return; }

	if (tasks != NULL && (_showTasks || _showSamples)) {
		//BOOST_FOREACH (GraspTarget& target, tasks->getSubTasks()[0].getTargets()) {
		typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
		BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {
			RenderTargets::Target rt;
			rt.ctask = p.first; //&tasks->getSubTasks()[0];
			rt.ctarget = *p.second; //target;

			if (_showSuccesses
					&& rt.ctarget.getResult()->testStatus != GraspResult::Success)
				continue;

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
			} else {
				rt.color[0] = 1.0;
				rt.color[1] = 0.0;
				rt.color[2] = 0.0;
				rt.color[3] = 0.5;
			}

			rt.trans = wTo * rt.ctarget.pose;

			rtargets.push_back(rt);
		}
	}

	//cout << "Painting!" << endl;

	((RenderTargets*)_render.get())->setTargets(rtargets);
	getRobWorkStudio()->postUpdateAndRepaint();
}

void GraspPlugin::test() {

	BOOST_FOREACH (Gripper::Ptr g, _gripperList) {
		// load gripper
		_gripper = g;
		updateGripper();

		string filename = _gripper->getName() + ".png";
		log().info() << "Saving image " + filename + "..." << endl;
		getRobWorkStudio()->saveViewGL(QString::fromStdString(filename));
	}

}

Q_EXPORT_PLUGIN (GraspPlugin);
