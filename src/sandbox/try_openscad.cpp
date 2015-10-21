#include <iostream>
#include <string>
#include <sstream>
#include <rw/RobWork.hpp>
#include <geometry/OpenSCADGenerator.hpp>
#include <rw/loaders/model3d/STLFile.hpp>
#include "math/Types.hpp"
#include "parametrization/ParametrizationTranslator.hpp"

using namespace std;
using namespace rw;
using namespace rw::geometry;
using namespace rw::common;
using namespace rw::loaders;
using namespace rw::math;
using namespace gripperz::geometry;
using namespace gripperz::parametrization;

int main(int argc, char* argv[]) {
    /* INITIALIZE ROBWORK */
    Math::seed();
    RobWork::getInstance()->initialize();
    Log::log().setLevel(Log::Debug);

    Parametrization::Ptr parametrization = new Parametrization(Parametrization::ParameterList{
        {"length", 0.1},
        {"width", 0.025},
        {"depth", 0.02},
        {"chfdepth", 0},
        {"chfangle", 45},
        {"cutpos", 0.075},
        {"cutdepth", 0.01},
        {"cutangle", 90},
        {"cuttilt", 0}
    });

    ParametrizedMeshGenerator::Ptr generator = new OpenSCADGenerator("../data/scad/standard_finger.scad");
    generator->setParametrization(parametrization);

    Parametrization::ParameterNameList parameter_names = parametrization->getParameterNameList();
    ParametrizationTranslator::Ptr translator = new ParametrizationTranslator(parameter_names);

    vector<double> values = translator->parametrizationToVector(parametrization);
    cout << values << endl;

    values[0] = 0.2;
    translator->applyVectorToParametrization(parametrization, values);
    cout << values << endl;

    TriMesh::Ptr mesh = generator->createMesh();

    STLFile::save(*mesh, "mesh.stl");

    return 0;
}

