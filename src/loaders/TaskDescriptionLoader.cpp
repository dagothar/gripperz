/**
 * @file TaskDescription.cpp
 * @author Adam Wolniakowski
 * @date 03-07-2015
 */

#include "TaskDescriptionLoader.hpp"

#include <iostream>
#include <string>
#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <boost/algorithm/string.hpp>


#define DEBUG rw::common::Log::debugLog()


using namespace std;
using namespace gripperz::util;
using namespace gripperz::context;
using namespace gripperz::loaders;
using boost::algorithm::trim;
using namespace boost::numeric;
using namespace boost::property_tree;
USE_ROBWORK_NAMESPACE
        using namespace robwork;
USE_ROBWORKSIM_NAMESPACE
        using namespace robworksim;

TaskDescription::Ptr TaskDescriptionLoader::readTaskDescription(PTree& tree, rwsim::dynamics::DynamicWorkCell::Ptr dwc) {
    TaskDescription::Ptr task = ownedPtr(new TaskDescription(dwc));

    if (task->getWorkCell() == NULL) {
        RW_THROW(" NULL wc");
    }

    DEBUG << "Loading task description..." << endl;

    readTarget(tree.get_child("Target"), task);
    readGripper(tree.get_child("Gripper"), task);
    readInterferenceObjects(tree.get_child("InterferenceObjects"), task);
    readLimits(tree.get_child("Limits"), task);

    DEBUG << "- prefiltering distance: ";
    PTree& node1 = tree.get_child("PrefilteringDistance");
    Q prefDist = XMLHelpers::readQ(node1);
    prefDist(1) *= Deg2Rad;
    prefDist(2) *= Deg2Rad;
    task->setPrefilteringDistance(prefDist);
    DEBUG << prefDist << endl;

    DEBUG << "- coverage distance: ";
    PTree& node2 = tree.get_child("CoverageDistance");
    Q covDist = XMLHelpers::readQ(node2);
    covDist(1) *= Deg2Rad;
    covDist(2) *= Deg2Rad;
    task->setCoverageDistance(covDist);
    DEBUG << covDist << endl;

    DEBUG << "- teach distance: ";
    PTree& node3 = tree.get_child("TeachDistance");
    Q teachDist = XMLHelpers::readQ(node3);
    teachDist(3) *= Deg2Rad;
    teachDist(4) *= Deg2Rad;
    task->setTeachDistance(teachDist);
    DEBUG << teachDist << endl;

    boost::optional<PTree&> node4 = tree.get_child_optional("HintGrasps");
    if (node4) {
        readHints(*node4, task);
    }

    boost::optional<PTree&> node6 = tree.get_child_optional("Alignment");
    if (node6) {
        readAlignment(*node6, task);
    }

    return task;
}

void TaskDescriptionLoader::readTarget(PTree& tree, TaskDescription::Ptr task) {
    string targetName = tree.get_value<string>();
    trim(targetName);

    DEBUG << "- target name: [" << targetName << "]" << endl;

    Object::Ptr targetObject = task->getWorkCell()->findObject(targetName);
    if (!targetObject) {
        RW_THROW("Cannot find target object!");
    }

    task->setTargetObject(targetObject);
    task->setTargetFrame(targetObject->getBase());
}

void TaskDescriptionLoader::readGripper(PTree& tree, TaskDescription::Ptr task) {
    DEBUG << "- gripper" << endl;

    // read name
    PTree& nameNode = tree.get_child("Name");
    string gripperName = nameNode.get_value<string>();
    trim(gripperName);
    DEBUG << "\t name: [" << gripperName << "]" << endl;
    task->setGripperID(gripperName);

    rw::models::TreeDevice::Ptr gripperDevice = task->getWorkCell()->findDevice<TreeDevice>(gripperName);
    if (!gripperDevice) {
        RW_THROW("Gripper device not found!");
    }
    task->setGripperDevice(gripperDevice);

    rwsim::dynamics::RigidDevice::Ptr dynamicDevice = task->getDynamicWorkCell()->findDevice<RigidDevice>(gripperName);
    if (!dynamicDevice) {
        RW_THROW("Gripper dynamic device not found!");
    }
    task->setGripperDynamicDevice(dynamicDevice);

    // read tcp frame
    PTree& tcpNode = tree.get_child("TCP");
    string tcpName = tcpNode.get_value<string>();
    trim(tcpName);
    DEBUG << "\t tcp frame: [" << tcpName << "]" << endl;
    rw::kinematics::MovableFrame::Ptr tcpFrame = task->getWorkCell()->findFrame<MovableFrame>(tcpName);
    if (!tcpFrame) {
        RW_THROW("Gripper TCP frame not found!");
    }
    task->setGripperTCP(tcpFrame);


    // read movable frame
    PTree& movNode = tree.get_child("Movable");
    string movableName = movNode.get_value<string>();
    trim(movableName);
    DEBUG << "\t movable frame: [" << movableName << "]" << endl;
    MovableFrame* movableFrame = task->getWorkCell()->findFrame<MovableFrame>(movableName);
    task->setGripperMovable(movableFrame);
    if (!movableFrame) {
        RW_THROW("Gripper movable frame not found!");
    }

    // read controller
    PTree& ctrlrNode = tree.get_child("Controller");
    string ctrlrName = ctrlrNode.get_value<string>();
    trim(ctrlrName);
    DEBUG << "\t controller: [" << ctrlrName << "]" << endl;
    task->setControllerID(ctrlrName);
}

void TaskDescriptionLoader::readInterferenceObjects(PTree& tree, TaskDescription::Ptr task) {
    DEBUG << "- interference objects" << endl;

    for (CI p = tree.begin(); p != tree.end(); ++p) {
        if (p->first == "Object") {
            string objName = p->second.get_value<string>();
            trim(objName);

            DEBUG << "\t object: [" << objName << "]" << endl;

            Object::Ptr obj = task->getWorkCell()->findObject(objName);
            if (obj == NULL) {
                RW_THROW("Interference object not found!");
            }

            task->addInterferenceObject(obj);
        }
    }
}

void TaskDescriptionLoader::readLimits(PTree& tree, TaskDescription::Ptr task) {
    DEBUG << "- limits" << endl;

    DEBUG << "\tInterference limit: ";
    PTree& intChild = tree.get_child("Interference");
    double intLimit = XMLHelpers::readDouble(intChild);
    task->setInterferenceLimit(intLimit);
    DEBUG << intLimit << endl;

    DEBUG << "\tWrench limit: ";
    PTree& wrenchChild = tree.get_child("Wrench");
    double wrenchLimit = XMLHelpers::readDouble(wrenchChild);
    task->setWrenchLimit(wrenchLimit);
    DEBUG << wrenchLimit << endl;

    boost::optional<PTree&> stressNode = tree.get_child_optional("Stress");
    if (stressNode) {
        DEBUG << "\tStress limit: ";
        double stressLimit = XMLHelpers::readDouble(stressNode.get());
        task->setStressLimit(stressLimit);
        DEBUG << stressLimit << endl;
    } else {
        task->setStressLimit(0.0);
    }

    boost::optional<PTree&> volumeNode = tree.get_child_optional("Volume");
    if (volumeNode) {
        DEBUG << "\tVolume limit: ";
        double volumeLimit = XMLHelpers::readDouble(volumeNode.get());
        task->setVolumeLimit(volumeLimit);
        DEBUG << volumeLimit << endl;
    } else {
        task->setVolumeLimit(0.02);
    }
}

TaskDescription::Ptr TaskDescriptionLoader::load(const std::string& filename, rwsim::dynamics::DynamicWorkCell::Ptr dwc) {
    TaskDescription::Ptr task;

    try {
        PTree tree;
        read_xml(filename, tree);

        task = readTaskDescription(tree.get_child("TaskDescription"), dwc);
    } catch (const ptree_error& e) {
        RW_THROW(e.what());
    }

    return task;
}

void TaskDescriptionLoader::readHints(gripperz::util::PTree& tree, TaskDescription::Ptr task) {
    DEBUG << "- hints" << endl;

    for (CI p = tree.begin(); p != tree.end(); ++p) {
        if (p->first == "Grasp") readGrasp(p->second, task);
    }
}

void TaskDescriptionLoader::readGrasp(gripperz::util::PTree& tree, TaskDescription::Ptr task) {
    DEBUG << "\tGrasp: ";
    Q posq = XMLHelpers::readQ(tree.get_child("Pos"));
    Q rpyq = XMLHelpers::readQ(tree.get_child("RPY"));
    DEBUG << "pos=" << posq << " rot=" << rpyq << endl;

    Vector3D<> pos(posq[0], posq[1], posq[2]);
    RPY<> rpy(rpyq[0], rpyq[1], rpyq[2]);
    task->addHint(Transform3D<>(pos, rpy.toRotation3D()));
}

void TaskDescriptionLoader::readAlignment(gripperz::util::PTree& tree, TaskDescription::Ptr task) {
    double filteringDistance = XMLHelpers::readDouble(tree.get_child("FilteringDistance"));
    task->setAlignmentFilteringDistance(filteringDistance);
}

void TaskDescriptionLoader::save(const TaskDescription::Ptr td, const std::string& filename) {
    PTree tree;


    // put target
    tree.put("TaskDescription.Target", td->getTargetObject()->getName());

    // save gripper information
    tree.put("TaskDescription.Gripper.Name", td->getGripperDevice()->getName());
    tree.put("TaskDescription.Gripper.TCP", td->getGripperTCP()->getName());
    tree.put("TaskDescription.Gripper.Movable", td->getGripperMovable()->getName());
    tree.put("TaskDescription.Gripper.Controller", td->getControllerID());

    // save interference objects

    BOOST_FOREACH(rw::models::Object::Ptr obj, td->getInterferenceObjects()) {
        PTree node;

        node.put_value(obj->getName());

        tree.add_child("TaskDescription.InterferenceObjects.Object", node);
    }

    // save limits
    tree.put("TaskDescription.Limits.Interference", td->getInterferenceLimit());
    tree.put("TaskDescription.Limits.Wrench", td->getWrenchLimit());
    tree.put("TaskDescription.Limits.Stress", td->getStressLimit());
    tree.put("TaskDescription.Limits.Volume", td->getVolumeLimit());

    // save teach & coverage distance
    Q teachDist = td->getTeachDistance();
    teachDist(3) *= Rad2Deg;
    teachDist(4) *= Rad2Deg;
    tree.put("TaskDescription.TeachDistance", XMLHelpers::QToString(teachDist));
    Q preDist = td->getPrefilteringDistance();
    preDist(1) *= Rad2Deg;
    preDist(2) *= Rad2Deg;
    tree.put("TaskDescription.PrefilteringDistance", XMLHelpers::QToString(preDist));
    Q covDist = td->getCoverageDistance();
    covDist(1) *= Rad2Deg;
    covDist(2) *= Rad2Deg;
    tree.put("TaskDescription.CoverageDistance", XMLHelpers::QToString(covDist));

    // save grasp hints

    BOOST_FOREACH(Transform3D<> hint, td->getHints()) {
        PTree node;

        node.put("Pos", XMLHelpers::QToString(Q(3, hint.P()[0], hint.P()[1], hint.P()[2])));
        RPY<> rpy(hint.R());
        node.put("RPY", XMLHelpers::QToString(Q(3, rpy[0], rpy[1], rpy[2])));

        tree.add_child("TaskDescription.HintGrasps.Grasp", node);
    }

    // save alignment ransac parameters
    {
        PTree node0;
        double filteringDistance = td->getAlignmentFilteringDistance();
        node0.put("FilteringDistance", boost::lexical_cast<string>(filteringDistance));

        tree.add_child("TaskDescription.Alignment", node0);
    }

    // save to XML
    try {
        boost::property_tree::xml_writer_settings<char> settings('\t', 1);
        write_xml(filename, tree, std::locale(), settings);
    } catch (const ptree_error& e) {
        // Convert from parse errors to RobWork errors.
        RW_THROW(e.what());
    }
}
