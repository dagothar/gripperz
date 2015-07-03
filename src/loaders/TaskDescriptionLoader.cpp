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


TaskDescription::Ptr TaskDescriptionLoader::readTaskDescription(PTree& tree, rwsim::dynamics::DynamicWorkCell::Ptr dwc)
{
	TaskDescription::Ptr task = ownedPtr(new TaskDescription(dwc));
	
	if (task->getWorkCell() == NULL) {
		RW_THROW(" NULL wc");
	}
	
	DEBUG << "Loading task description..." << endl;
	
	readTarget(tree.get_child("Target"), task);
	readGripper(tree.get_child("Gripper"), task);
	readInterferenceObjects(tree.get_child("InterferenceObjects"), task);
	readLimits(tree.get_child("Limits"), task);
	
	DEBUG << "- weights" << endl;
	readQualities(tree.get_child("Weights"), task->getWeights());
	
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



void TaskDescriptionLoader::readTarget(PTree& tree, TaskDescription::Ptr task)
{
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



void TaskDescriptionLoader::readGripper(PTree& tree, TaskDescription::Ptr task)
{
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
	rw::kinematics::Frame* tcpFrame = task->getWorkCell()->findFrame(tcpName);
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
	if (movableFrame) {
		RW_THROW("Gripper movable frame not found!");
	}
		
	// read controller
	PTree& ctrlrNode = tree.get_child("Controller");
	string ctrlrName = ctrlrNode.get_value<string>();
	trim(ctrlrName);
	DEBUG << "\t controller: [" << ctrlrName << "]" << endl;
	task->setControllerID(ctrlrName);
}



void TaskDescriptionLoader::readInterferenceObjects(PTree& tree, TaskDescription::Ptr task)
{
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



void TaskDescriptionLoader::readLimits(PTree& tree, TaskDescription::Ptr task)
{
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
}



void TaskDescriptionLoader::readQualities(PTree& tree, TaskDescription::Qualities& q)
{	
	DEBUG << "\tCoverage: ";
	PTree& node2 = tree.get_child("Coverage");
	q.coverage = XMLHelpers::readDouble(node2);
	DEBUG << q.coverage << endl;
	
	DEBUG << "\tSuccess ratio: ";
	PTree& node3 = tree.get_child("SuccessRatio");
	q.success = XMLHelpers::readDouble(node3);
	DEBUG << q.success << endl;
	
	DEBUG << "\tWrench: ";
	PTree& node4 = tree.get_child("Wrench");
	q.wrench = XMLHelpers::readDouble(node4);
	DEBUG << q.wrench << endl;
	
	// read optional stress & volume weights
	boost::optional<PTree&> stressNode = tree.get_child_optional("Stress");
	if (stressNode) {
		DEBUG << "\tStress: ";
		q.stress = XMLHelpers::readDouble(stressNode.get());
		DEBUG << q.stress << endl;
	} else {
		q.stress = 1.0;
	}
	
	boost::optional<PTree&> volumeNode = tree.get_child_optional("Volume");
	if (volumeNode) {
		DEBUG << "\tVolume: ";
		q.volume = XMLHelpers::readDouble(volumeNode.get());
		DEBUG << q.volume << endl;
	} else {
		q.volume = 0.0;
	}
	
	boost::optional<PTree&> alignmentNode = tree.get_child_optional("Alignment");
	if (alignmentNode) {
		DEBUG << "\tAlignment: ";
		q.alignment = XMLHelpers::readDouble(alignmentNode.get());
		DEBUG << q.alignment << endl;
	} else {
		q.alignment = 0.0;
	}
}



TaskDescription::Ptr TaskDescriptionLoader::load(const std::string& filename, rwsim::dynamics::DynamicWorkCell::Ptr dwc)
{
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



void TaskDescriptionLoader::readHints(gripperz::util::PTree& tree, TaskDescription::Ptr task)
{
	DEBUG << "- hints" << endl;
	
	for (CI p = tree.begin(); p != tree.end(); ++p) {
		if (p->first == "Grasp") readGrasp(p->second, task);
	}
}



void TaskDescriptionLoader::readGrasp(gripperz::util::PTree& tree, TaskDescription::Ptr task)
{
	DEBUG << "\tGrasp: ";
	Q posq = XMLHelpers::readQ(tree.get_child("Pos"));
	Q rpyq = XMLHelpers::readQ(tree.get_child("RPY"));
	DEBUG << "pos=" << posq << " rot=" << rpyq << endl;
	
	Vector3D<> pos(posq[0], posq[1], posq[2]);
	RPY<> rpy(rpyq[0], rpyq[1], rpyq[2]);
	task->addHint(Transform3D<>(pos, rpy.toRotation3D()));
}


void TaskDescriptionLoader::readAlignment(gripperz::util::PTree& tree, TaskDescription::Ptr task)
{
	DEBUG << "\tAlignment model: ";
	
	// read pose 0D parameters
	TaskDescription::AlignmentModelParameters& pose0params = task->getAlignmentModelParameters(0);
	PTree& pose0tag = tree.get_child("StablePose0D");
	pose0params.iterations = XMLHelpers::readInt(pose0tag.get_child("Iterations"));
	pose0params.minInliers = XMLHelpers::readInt(pose0tag.get_child("MinInliers"));
	pose0params.dataThreshold = XMLHelpers::readDouble(pose0tag.get_child("DataThreshold"));
	pose0params.modelThreshold = XMLHelpers::readDouble(pose0tag.get_child("ModelThreshold"));
	pose0params.weight = XMLHelpers::readDouble(pose0tag.get_child("Weight"));
	
	// read pose 0D parameters
	TaskDescription::AlignmentModelParameters& pose1params = task->getAlignmentModelParameters(1);
	PTree& pose1tag = tree.get_child("StablePose1D");
	pose1params.iterations = XMLHelpers::readInt(pose1tag.get_child("Iterations"));
	pose1params.minInliers = XMLHelpers::readInt(pose1tag.get_child("MinInliers"));
	pose1params.dataThreshold = XMLHelpers::readDouble(pose1tag.get_child("DataThreshold"));
	pose1params.modelThreshold = XMLHelpers::readDouble(pose1tag.get_child("ModelThreshold"));
	pose1params.weight = XMLHelpers::readDouble(pose1tag.get_child("Weight"));
}



void TaskDescriptionLoader::save(const TaskDescription::Ptr td, const std::string& filename)
{
	PTree tree;
	
	
	// put target
	tree.put("TaskDescription.Target", td->getTargetObject()->getName());
	
	// save gripper information
	tree.put("TaskDescription.Gripper.Name", td->getGripperDevice()->getName());
	tree.put("TaskDescription.Gripper.TCP", td->getGripperTCP()->getName());
	tree.put("TaskDescription.Gripper.Movable", td->getGripperMovable()->getName());
	tree.put("TaskDescription.Gripper.Controller", td->getControllerID());
	
	// save interference objects
	BOOST_FOREACH (rw::models::Object::Ptr obj, td->getInterferenceObjects()) {
		PTree node;
		
		node.put_value(obj->getName());
		
		tree.add_child("TaskDescription.InterferenceObjects.Object", node);
	}
	
	// save limits
	tree.put("TaskDescription.Limits.Interference", td->getInterferenceLimit());
	tree.put("TaskDescription.Limits.Wrench", td->getWrenchLimit());
	tree.put("TaskDescription.Limits.Stress", td->getStressLimit());
	
	// save weights
	tree.put("TaskDescription.Weights.Coverage", td->getWeights().coverage);
	tree.put("TaskDescription.Weights.SuccessRatio", td->getWeights().success);
	tree.put("TaskDescription.Weights.Wrench", td->getWeights().wrench);
	tree.put("TaskDescription.Weights.Stress", td->getWeights().stress);
	tree.put("TaskDescription.Weights.Volume", td->getWeights().volume);
	
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
	BOOST_FOREACH (Transform3D<> hint, td->getHints()) {
		PTree node;
		
		node.put("Pos", XMLHelpers::QToString(Q(3, hint.P()[0], hint.P()[1], hint.P()[2])));
		RPY<> rpy(hint.R());
		node.put("RPY", XMLHelpers::QToString(Q(3, rpy[0], rpy[1], rpy[2])));
		
		tree.add_child("TaskDescription.HintGrasps.Grasp", node);
	}
	
	// save alignment ransac parameters
	{
		PTree node0;
		TaskDescription::AlignmentModelParameters pose0Params = td->getAlignmentModelParameters(0);
		node0.put("Iterations", boost::lexical_cast<string>(pose0Params.iterations));
		node0.put("MinInliers", boost::lexical_cast<string>(pose0Params.minInliers));
		node0.put("DataThreshold", boost::lexical_cast<string>(pose0Params.dataThreshold));
		node0.put("ModelThreshold", boost::lexical_cast<string>(pose0Params.modelThreshold));
		node0.put("Weight", boost::lexical_cast<string>(pose0Params.weight));
		
		PTree node1;
		TaskDescription::AlignmentModelParameters pose1Params = td->getAlignmentModelParameters(1);
		node1.put("Iterations", boost::lexical_cast<string>(pose1Params.iterations));
		node1.put("MinInliers", boost::lexical_cast<string>(pose1Params.minInliers));
		node1.put("DataThreshold", boost::lexical_cast<string>(pose1Params.dataThreshold));
		node1.put("ModelThreshold", boost::lexical_cast<string>(pose1Params.modelThreshold));
		node1.put("Weight", boost::lexical_cast<string>(pose1Params.weight));
		
		tree.add_child("TaskDescription.Alignment.StablePose0D", node0);
		tree.add_child("TaskDescription.Alignment.StablePose1D", node1);
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
