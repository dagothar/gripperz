#include "gripper_design.hpp"
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

gripper_design::gripper_design() :
RobWorkStudioPlugin("gripper_design", QIcon(":/design_icon.png")) {

    _ui.setupUi(this);
    setupGUI();

    makeGripper();
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
        _state = _wc->getDefaultState();

    } catch (const rw::common::Exception& e) {
        QMessageBox::critical(NULL, "RW Exception", e.what());
    }
}

void gripper_design::close() {
}

void gripper_design::setupGUI() {
    connect(_ui.loadButton, SIGNAL(clicked()), this, SLOT(loadGripper()));
    connect(_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveGripper()));
    //connect(_ui.updateButton, SIGNAL(clicked()), this, SLOT(updateGripper()));
    connect(_ui.applyButton, SIGNAL(clicked()), this, SLOT(applyGripper()));
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

void gripper_design::loadGripper() {
    QString filename = QFileDialog::getOpenFileName(this, "Open file", "", tr("Gripper files (*.xml)"));

    if (filename.isEmpty()) {
        return;
    }

    log().info() << "Loading gripper from: " << filename.toStdString() << "\n";

    StandardGripperLoader::Ptr loader = new StandardGripperLoader();
    _gripper = loader->load(filename.toStdString()).cast<StandardGripper>();

    updateGUI();
}

void gripper_design::saveGripper() {
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", tr("Gripper files (*.xml)"));

    if (filename.isEmpty()) {
        return;
    }

    log().info() << "Saving gripper to: " << filename.toStdString() << "\n";

    StandardGripperLoader::Ptr loader = new StandardGripperLoader();
    loader->save(filename.toStdString(), _gripper);
}

void gripper_design::updateGripper() {
    _gripper->setName(_ui.nameEdit->text().toStdString());
    _gripper->setDeviceId(_ui.deviceEdit->text().toStdString());
    _gripper->setDynamicDeviceId(_ui.ddeviceEdit->text().toStdString());
    _gripper->setTCPFrameId(_ui.tcpEdit->text().toStdString());
    _gripper->setMovableFrameId(_ui.movableEdit->text().toStdString());
    _gripper->setGraspControllerId(_ui.ctrlrEdit->text().toStdString());
    _gripper->setLeftFingerId(_ui.leftEdit->text().toStdString());
    _gripper->setRightFingerId(_ui.rightEdit->text().toStdString());
    _gripper->getMeshGenerator().cast<OpenSCADGenerator>()->setScriptFilename(_ui.templateEdit->text().toStdString());

    updateParametrization();
}

void gripper_design::applyGripper() {
    RW_ASSERT(_gripper != NULL);

    updateGripper();

    log().info() << "Registering gripper..." << endl;
    _gripper->registerWithContext(_wc, _dwc, _state);
    getRobWorkStudio()->setState(_state);
    log().info() << "Registered." << endl;

    log().info() << "Applying gripper modifications..." << endl;
    _gripper->applyModifications(_wc, _dwc, _state);

    getRobWorkStudio()->getWorkCellScene()->clearCache();
    getRobWorkStudio()->getWorkCellScene()->updateSceneGraph(_state);
    getRobWorkStudio()->setWorkcell(_wc);
    getRobWorkStudio()->setState(_state);
    log().info() << "Done." << endl;
}

void gripper_design::makeGripper() {
    log().info() << "Making new standard gripper..." << endl;

    _gripper = ownedPtr(new StandardGripper("gripper"));

    _gripper->setDeviceId("gripper");
    _gripper->setDynamicDeviceId("gripper");
    _gripper->setTCPFrameId("TCPgripper");
    _gripper->setMovableFrameId("gripper.Base");
    _gripper->setGraspControllerId("graspController");
    _gripper->setLeftFingerId("gripper.LeftFinger");
    _gripper->setRightFingerId("gripper.RightFinger");

    Parametrization::Ptr params = makeParametrization();
    _gripper->setParametrization(params);

    ParametrizedMeshGenerator::Ptr gen = ownedPtr(new OpenSCADGenerator("${GRIPPERZ_ROOT}/data/scad/prismatic_cutout.scad"));
    _gripper->setMeshGenerator(gen);

    updateGUI();
}

gripperz::parametrization::Parametrization::Ptr gripper_design::makeParametrization() {
    Parametrization::Ptr p = ownedPtr(new Parametrization(Parametrization::ParameterList{
        {"length", 0.1},
        {"width", 0.025},
        {"depth", 0.02},
        {"chfdepth", 0},
        {"chfangle", 45},
        {"cutpos", 0.075},
        {"cutoffset", 0},
        {"cutdepth", 0.01},
        {"cutwidth", 0.01},
        {"cutangle", 90},
        {"cutangle2", 90},
        {"tcpoffset", 0.075},
        {"jawdist", 0},
        {"stroke", 0.05},
        {"force", 50}
    }));

    return p;
}

void gripper_design::updateGUI() {
    _ui.nameEdit->setText(QString::fromStdString(_gripper->getName()));
    _ui.deviceEdit->setText(QString::fromStdString(_gripper->getDeviceId()));
    _ui.ddeviceEdit->setText(QString::fromStdString(_gripper->getDynamicDeviceId()));
    _ui.tcpEdit->setText(QString::fromStdString(_gripper->getTCPFrameId()));
    _ui.movableEdit->setText(QString::fromStdString(_gripper->getMovableFrameId()));
    _ui.ctrlrEdit->setText(QString::fromStdString(_gripper->getGraspControllerId()));
    _ui.leftEdit->setText(QString::fromStdString(_gripper->getLeftFingerId()));
    _ui.rightEdit->setText(QString::fromStdString(_gripper->getRightFingerId()));
    _ui.templateEdit->setText(QString::fromStdString(_gripper->getMeshGenerator().cast<OpenSCADGenerator>()->getScriptFilename()));

    updateParameterTable();
}

void gripper_design::updateParameterTable() {
    /* clear rows */
    //    while (_ui.parametersTable->rowCount() > 0) {
    //        _ui.parametersTable->removeRow(_ui.parametersTable->rowCount() - 1);
    //    }
    _ui.parametersTable->clearContents();

    /* populate table */
    int row = 0;
    Parametrization::ParameterList params = _gripper->getParametrization()->getParameterList();
    _ui.parametersTable->setRowCount(params.size());

    BOOST_FOREACH(const Parameter& p, params) {
        QTableWidgetItem* name = new QTableWidgetItem();
        name->setText(QString::fromStdString(p.first));
        _ui.parametersTable->setItem(row, 0, name);

        QTableWidgetItem* val = new QTableWidgetItem();
        val->setText(QString::number(p.second));
        _ui.parametersTable->setItem(row, 1, val);

        ++row;
    }
}

void gripper_design::updateParametrization() {
    RW_ASSERT(_gripper != NULL);

    Parametrization::Ptr params = ownedPtr(new Parametrization());
    for (int i = 0; i < _ui.parametersTable->rowCount(); ++i) {
        QTableWidgetItem* name = _ui.parametersTable->item(i, 0);
        QTableWidgetItem* val = _ui.parametersTable->item(i, 1);
        Parameter p{name->text().toStdString(), val->text().toDouble()};

        //log().info() << "Adding parameter: " << p.first << ": " << p.second << endl;
        params->addParameter(p);
    }

    _gripper->setParametrization(params);
}


Q_EXPORT_PLUGIN(gripper_design);
