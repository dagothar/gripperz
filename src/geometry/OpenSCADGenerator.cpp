/* 
 * File:   OpenSCADGenerator.cpp
 * Author: dagothar
 * 
 * Created on October 21, 2015, 3:55 PM
 */

#include "OpenSCADGenerator.hpp"
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <rw/loaders/model3d/STLFile.hpp>

#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()

using namespace std;
using namespace gripperz::geometry;
using namespace gripperz::parametrization;
using namespace rw::geometry;
using namespace rw::loaders;
using namespace boost::filesystem;

OpenSCADGenerator::OpenSCADGenerator(const std::string& script, const std::string& temporaryFilename, const std::string& command) :
_scriptFilename(script),
_temporaryFilename(temporaryFilename),
_command(command) {

}

OpenSCADGenerator::~OpenSCADGenerator() {
}

rw::geometry::TriMesh::Ptr OpenSCADGenerator::createMesh() {
    /* construct command */
    stringstream sstr;
    sstr << _command;
    sstr << " -o " << _temporaryFilename << " ";
    sstr << _scriptFilename << " ";

    BOOST_FOREACH(const Parameter& p, getParametrization()->getParameterList()) {
        sstr << "-D '" << p.first << "=" << p.second << ";' ";
    }

    string command = sstr.str();

    DEBUG << "Command: " << command;

    /* call script */
    if (exists(_temporaryFilename)) {
        remove(_temporaryFilename);
    }

    int return_code = system(command.c_str());
    if (return_code != 0) {
        RW_THROW("Geometry generation failed!");
    }

    /* load result */
    TriMesh::Ptr mesh = STLFile::load(_temporaryFilename);

    /* remove temporary file */
    if (exists(_temporaryFilename)) {
        remove(_temporaryFilename);
    }

    return mesh;
}
