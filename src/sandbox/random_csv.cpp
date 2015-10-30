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
    string output_filename;
    int n_poses;
} CONFIG;

bool parse_cli(int argc, char* argv[], Configuration& config) {

    string usage =
            "Generates random SO3 data\n\n"
            "Usage:\n"
            "\trandom_csv CSV_FILE\n\n";

    options_description desc("Allowed options");
    desc
            .add_options()
            ("help,h", "help message")
            ("n_poses,n", value<int>(&config.n_poses)->default_value(100), "number of poses to generate")
            ("output", value<string>(&config.output_filename)->required(), "destination CSV filename");
    positional_options_description p;
    p
            .add("output", 2);

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

    return true;
}

int main(int argc, char* argv[]) {
    initialize();

    if (!parse_cli(argc, argv, CONFIG)) {
        return -1;
    }

    /* save csv file */
    ofstream out(CONFIG.output_filename);
    out << "x, y, z, a" << endl;
    
    for (int i = 0; i < CONFIG.n_poses; ++i) {
        RPY<> rpy(Math::ran(-Pi, Pi), Math::ran(-Pi, Pi), Math::ran(-Pi, Pi));
        Rotation3D<> r = rpy.toRotation3D();

        Vector3D<> vx = r * Vector3D<>::x();
        Vector3D<> vy = r * Vector3D<>::y();
        Vector3D<> vz = r * Vector3D<>::z();

        out << vx[0] << ", " << vx[1] << ", " << vx[2] << ", " << 0 << endl;
        out << vy[0] << ", " << vy[1] << ", " << vy[2] << ", " << 1 << endl;
        out << vz[0] << ", " << vz[1] << ", " << vz[2] << ", " << 2 << endl;
    }

    out.close();

    return 0;
}
