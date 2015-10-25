/**
 * @file gripper_evaluate.cpp
 * @author Adam Wolniakowski
 * @date 29-07-2015
 */

#include <iostream>
#include <fstream>
#include <string>
#include <rw/rw.hpp>
#include <rw/common/Log.hpp>
#include <rw/math/Transform3D.hpp>
#include <rw/math/RPY.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>

using namespace std;
using namespace boost::program_options;
using namespace rw;
using namespace rw::common;
using namespace rw::math;
using namespace rwlibs::task;

void initialize() {
    RobWork::getInstance()->initialize();
    Log::log().setLevel(Log::Info);
}

struct Configuration {
    string input_filename;
    string output_filename;
    Transform3D<> offset;
} CONFIG;

bool parse_cli(int argc, char* argv[], Configuration& config) {

    string offset;

    string usage =
            "Translates grasps saved in rwtask file to a csv file format\n\n"
            "Usage:\n"
            "\trwtask_to_csv RWTASK_FILE CSV_FILE [-o \"X Y Z ROLL PITCH YAW\"] \n\n";

    options_description desc("Allowed options");
    desc
            .add_options()
            ("help,h", "help message")
            ("input", value<string>(&config.input_filename)->required(), "rwtask XML file")
            ("output", value<string>(&config.output_filename)->required(), "destination CSV filename")
            ("offset, o", value<string>(&offset), "grasps position offset: \"x y z roll pitch yaw\"");
    positional_options_description p;
    p
            .add("input", 1).add("output", 2);

    variables_map vm;

    try {
        store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << usage << endl;
            cout << desc << endl;
            return false;
        }

    } catch (exception& e) {
        cout << usage << endl;
        cout << desc << endl;
        return false;
    }

    if (!offset.empty()) {
        istringstream offset_sstr(offset);
        double x, y, z, roll, pitch, yaw;

        offset_sstr >> x >> y >> z >> roll >> pitch >> yaw;

        config.offset = Transform3D<>(
                Vector3D<>(x, y, z),
                RPY<>(roll * Deg2Rad, pitch * Deg2Rad, yaw * Deg2Rad).toRotation3D());
    }

    return true;
}

int rwStatusToSimpleStatus(int status) {
    switch (status) {
        case 14: // misaligned
            return 1;
        case 1: // success
            return 2;
        default: // uninitialized + failure
            return 0;
    }
}

int main(int argc, char* argv[]) {
    initialize();

    if (!parse_cli(argc, argv, CONFIG)) {
        return -1;
    }

    /* load rw tasks */
    GraspTask::Ptr grasps = GraspTask::load(CONFIG.input_filename);

    /* save csv file */
    ofstream out(CONFIG.output_filename);
    typedef pair<class GraspSubTask*, class GraspTarget*> TaskTarget;

    BOOST_FOREACH(const TaskTarget& tt, grasps->getAllTargets()) {
        GraspTarget* target = tt.second;
        Transform3D<> pose = inverse(CONFIG.offset) * target->pose;
        Vector3D<> p = pose.P();
        RPY<> r = RPY<>(pose.R());
        int status = rwStatusToSimpleStatus(target->getResult()->testStatus);

        out << p[0] << ", " << p[1] << ", " << p[2] << ", ";
        out << r[0] * Rad2Deg << ", " << r[1] * Rad2Deg << ", " << r[2] * Rad2Deg << ", ";
        out << status << endl;
    }

    out.close();

    return 0;
}
