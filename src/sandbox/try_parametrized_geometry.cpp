#include <iostream>
#include <string>
#include <sstream>
#include <geometry/OldFingerGenerator.hpp>
#include <rw/loaders/model3d/STLFile.hpp>
#include "math/Types.hpp"
#include "parametrization/ParametrizationTranslator.hpp"

using namespace std;
using namespace rw::geometry;
using namespace rw::common;
using namespace rw::loaders;
using namespace rw::math;
using namespace gripperz::geometry;
using namespace gripperz::parametrization;

int main(int argc, char* argv[]) {
    
    ParametrizedMeshGenerator::Ptr generator = new OldFingerGenerator();
    Parametrization::Ptr parametrization = generator->getParametrization();
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

