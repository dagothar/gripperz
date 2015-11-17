/**
 * @file MapGripperBuilder.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#include "MapGripperBuilder.hpp"

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace gripperz::models;
using namespace rw::common;
using namespace rw::math;
using namespace std;

MapGripperBuilder::MapGripperBuilder(OldGripper::Ptr prototype, const std::vector<ParameterName>& parameters) :
_prototype(prototype),
_parameters(parameters) {
}

MapGripperBuilder::~MapGripperBuilder() {
}

void MapGripperBuilder::applyParameter(OldGripper::Ptr gripper, ParameterName name, double value) {

    switch (name) {
        case Length:
            gripper->setLength(value);
            break;

        case Width:
            gripper->setWidth(value);
            break;

        case Depth:
            gripper->setDepth(value);
            break;

        case ChfDepth:
            gripper->setChfDepth(value);
            break;

        case ChfAngle:
            gripper->setChfAngle(value);
            break;

        case CutDepth:
            gripper->setCutDepth(value);
            break;

        case CutAngle:
            gripper->setCutAngle(value);
            break;

        case CutTilt:
            gripper->setCutTilt(value);
            break;

        case TcpOffset:
            gripper->setTcpOffset(value);
            break;

        case Force:
            gripper->setForce(value);
            break;

        case Stroke:
            gripper->setStroke(value);
            break;

        case Jawdist:
            gripper->setJawdist(value);
            break;

        default:
            RW_WARN("Parameter not implemented!");
    }
}

OldGripper::Ptr MapGripperBuilder::vectorToGripper(const std::vector<double>& params) {
    if (params.size() != _parameters.size()) {
        RW_THROW("Parameters vector size mismatch!");
    }

    OldGripper::Ptr gripper = ownedPtr(new OldGripper(*_prototype));

    for (unsigned i = 0; i < params.size(); ++i) {
        applyParameter(gripper, _parameters[i], params[i]);
    }

    return gripper;
}

double MapGripperBuilder::fetchParameter(OldGripper::Ptr gripper, ParameterName name) {

    switch (name) {
        case Length:
            return gripper->getLength();

        case Width:
            return gripper->getWidth();

        case Depth:
            return gripper->getDepth();

        case ChfDepth:
            return gripper->getChfDepth();

        case ChfAngle:
            return gripper->getChfAngle();

        case CutDepth:
            return gripper->getCutDepth();

        case CutAngle:
            return gripper->getCutAngle();

        case CutTilt:
            return gripper->getCutTilt();

        case TcpOffset:
            return gripper->getTcpOffset();

        case Force:
            return gripper->getForce();

        case Stroke:
            return gripper->getStroke();

        case Jawdist:
            return gripper->getJawdist();

        default:
            RW_WARN("Parameter not implemented!");
    }

    RW_THROW("Should never get here!");
    return 0.0;
}

std::vector<double> MapGripperBuilder::gripperToVector(OldGripper::Ptr gripper) {

    vector<double> params;

    for (unsigned i = 0; i < _parameters.size(); ++i) {
        params.push_back(fetchParameter(gripper, _parameters[i]));
    }

    return params;
}

std::string MapGripperBuilder::parameterNameToString(ParameterName parameter) {
    switch (parameter) {
        case Length:
            return "length";

        case Width:
            return "width";

        case Depth:
            return "depth";

        case ChfDepth:
            return "chfdepth";

        case ChfAngle:
            return "chfangle";

        case CutDepth:
            return "cutdepth";

        case CutAngle:
            return "cutangle";

        case CutTilt:
            return "cuttilt";

        case TcpOffset:
            return "tcpoffset";

        case Force:
            return "force";

        case Stroke:
            return "stroke";

        case Jawdist:
            return "jawdist";

        default:
            RW_WARN("Parameter not implemented!");
    };

    RW_THROW("Should not have got here");
    return "";
}
