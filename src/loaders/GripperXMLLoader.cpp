#include "GripperXMLLoader.hpp"
#include "models/GripperQualityFactory.hpp"

#include <string>
#include <sstream>
#include <rw/rw.hpp>
#include <rwsim/rwsim.hpp>
#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include <rw/loaders/model3d/STLFile.hpp>

#include <util/XMLHelpers.hpp>


#define DEBUG rw::common::Log::debugLog()


using namespace std;
USE_ROBWORK_NAMESPACE
        using namespace robwork;
using namespace rwsim;
using namespace rw::loaders;
using namespace boost::numeric;
using namespace boost::property_tree;
using namespace gripperz::util;
using namespace gripperz::loaders;
using namespace gripperz::models;

void readData(PTree& tree, OldGripper::Ptr gripper) {
    string deviceId = tree.get_child("deviceId").get_value<string>();
    string dynamicDeviceId = tree.get_child("dynamicDeviceId").get_value<string>();
    string tcpFrameId = tree.get_child("tcpFrameId").get_value<string>();
    string movableFrameId = tree.get_child("movableFrameId").get_value<string>();
    string graspControllerId = tree.get_child("graspControllerId").get_value<string>();
    string leftFingerId = tree.get_child("leftFingerId").get_value<string>();
    string rightFingerId = tree.get_child("rightFingerId").get_value<string>();
    
    gripper->setDeviceId(deviceId);
    gripper->setDynamicDeviceId(dynamicDeviceId);
    gripper->setTCPFrameId(tcpFrameId);
    gripper->setMovableFrameId(movableFrameId);
    gripper->setGraspControllerId(graspControllerId);
    gripper->setLeftFingerId(leftFingerId);
    gripper->setRightFingerId(rightFingerId);
}

void readParameters(PTree& tree, OldGripper::Ptr gripper) {
    double length = XMLHelpers::readDouble(tree.get_child("length"));
    double width = XMLHelpers::readDouble(tree.get_child("width"));
    double depth = XMLHelpers::readDouble(tree.get_child("depth"));
    double chfdepth = XMLHelpers::readDouble(tree.get_child("chfdepth"));
    double chfangle = XMLHelpers::readDouble(tree.get_child("chfangle"));
    double cutdepth = XMLHelpers::readDouble(tree.get_child("cutdepth"));
    double cutangle = XMLHelpers::readDouble(tree.get_child("cutangle"));
    double cuttilt = XMLHelpers::readDouble(tree.get_child("cuttilt"));
    double tcpoffset = XMLHelpers::readDouble(tree.get_child("tcpoffset"));
    double jawdist = XMLHelpers::readDouble(tree.get_child("jawdist"));
    double stroke = XMLHelpers::readDouble(tree.get_child("stroke"));
    double force = XMLHelpers::readDouble(tree.get_child("force"));
    double basex = XMLHelpers::readDouble(tree.get_child("basex"));
    double basey = XMLHelpers::readDouble(tree.get_child("basey"));
    double basez = XMLHelpers::readDouble(tree.get_child("basez"));

    gripper->setLength(length);
    gripper->setWidth(width);
    gripper->setDepth(depth);
    gripper->setChfDepth(chfdepth);
    gripper->setChfAngle(chfangle);
    gripper->setCutDepth(cutdepth);
    gripper->setCutAngle(cutangle);
    gripper->setCutTilt(cuttilt);
    gripper->setTcpOffset(tcpoffset);
    gripper->setJawdist(jawdist);
    gripper->setStroke(stroke);
    gripper->setForce(force);
    gripper->setBaseX(basex);
    gripper->setBaseY(basey);
    gripper->setBaseZ(basez);
}

void readResult(PTree& tree, OldGripper::Ptr gripper) {

    GripperQuality::Ptr quality = GripperQualityFactory::makeGripperQuality();

    // TODO: iterate
    quality->setIndex("success", XMLHelpers::readDouble(tree.get_child("success")));
    quality->setIndex("robustness", XMLHelpers::readDouble(tree.get_child("robustness")));
    quality->setIndex("alignment", XMLHelpers::readDouble(tree.get_child("alignment")));
    quality->setIndex("coverage", XMLHelpers::readDouble(tree.get_child("coverage")));
    quality->setIndex("wrench", XMLHelpers::readDouble(tree.get_child("wrench")));
    quality->setIndex("stress", XMLHelpers::readDouble(tree.get_child("stress")));
    quality->setIndex("volume", XMLHelpers::readDouble(tree.get_child("volume")));
    quality->setIndex("quality", XMLHelpers::readDouble(tree.get_child("objective")));

    gripper->setQuality(quality);

    DEBUG << "Read gripper quality:" << endl;
    //DEBUG << result; // << endl;
}

OldGripper::Ptr readGripper(PTree& tree) {
    OldGripper::Ptr gripper = ownedPtr(new OldGripper);

    readData(tree.get_child("data"), gripper);
    readParameters(tree.get_child("parameters"), gripper);
    readResult(tree.get_child("quality"), gripper);

    return gripper;
}

OldGripper::Ptr GripperXMLLoader::load(const std::string& filename) {
    OldGripper::Ptr gripper;

    try {
        // get path
        boost::filesystem::path p(filename);
        string gripperPath = p.parent_path().string() + "/";

        PTree tree;
        read_xml(filename, tree);

        gripper = readGripper(tree.get_child("gripper"));
        string name = tree.get_child("gripper").get_child("<xmlattr>.name").get_value<string>();

        gripper->setName(name);
    } catch (const ptree_error& e) {
        // Convert from parse errors to RobWork errors.
        RW_THROW(e.what());
    }

    return gripper;
}

void GripperXMLLoader::save(OldGripper::Ptr gripper, const std::string& filename) {
    PTree tree;

    /* save name */
    tree.put("gripper.<xmlattr>.name", gripper->getName());
    
    /* save data */
    tree.put("gripper.data.deviceId", gripper->getDeviceId());
    tree.put("gripper.data.dynamicDeviceId", gripper->getDynamicDeviceId());
    tree.put("gripper.data.tcpFrameId", gripper->getTCPFrameId());
    tree.put("gripper.data.movableFrameId", gripper->getMovableFrameId());
    tree.put("gripper.data.graspControllerId", gripper->getGraspControllerId());
    tree.put("gripper.data.leftFingerId", gripper->getLeftFingerId());
    tree.put("gripper.data.rightFingerId", gripper->getRightFingerId());

    /* save parameters */
    tree.put("gripper.parameters.length", gripper->getLength());
    tree.put("gripper.parameters.width", gripper->getWidth());
    tree.put("gripper.parameters.depth", gripper->getDepth());
    tree.put("gripper.parameters.chfdepth", gripper->getChfDepth());
    tree.put("gripper.parameters.chfangle", gripper->getChfAngle());
    tree.put("gripper.parameters.cutdepth", gripper->getCutDepth());
    tree.put("gripper.parameters.cutangle", gripper->getCutAngle());
    tree.put("gripper.parameters.cuttilt", gripper->getCutTilt());
    tree.put("gripper.parameters.tcpoffset", gripper->getTcpOffset());
    tree.put("gripper.parameters.jawdist", gripper->getJawdist());
    tree.put("gripper.parameters.stroke", gripper->getStroke());
    tree.put("gripper.parameters.force", gripper->getForce());
    tree.put("gripper.parameters.basex", gripper->getBaseX());
    tree.put("gripper.parameters.basey", gripper->getBaseY());
    tree.put("gripper.parameters.basez", gripper->getBaseZ());

    /* save qualities */
    GripperQuality::Ptr quality = gripper->getQuality();

    // TODO: iterate
    tree.put("gripper.quality.success", quality->getIndex("success"));
    tree.put("gripper.quality.robustness", quality->getIndex("robustness"));
    tree.put("gripper.quality.alignment", quality->getIndex("alignment"));
    tree.put("gripper.quality.coverage", quality->getIndex("coverage"));
    tree.put("gripper.quality.wrench", quality->getIndex("wrench"));
    tree.put("gripper.quality.stress", quality->getIndex("stress"));
    tree.put("gripper.quality.volume", quality->getIndex("volume"));
    tree.put("gripper.quality.objective", quality->getIndex("quality"));

    try {
        boost::property_tree::xml_writer_settings<char> settings('\t', 1);
        write_xml(filename, tree, std::locale(), settings);
    } catch (const ptree_error& e) {
        // Convert from parse errors to RobWork errors.
        RW_THROW(e.what());
    }
}
