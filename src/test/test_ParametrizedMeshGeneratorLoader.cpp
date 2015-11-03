#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ParametrizedMeshGeneratorLoaderTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <geometry/loaders/ParametrizedMeshGeneratorLoader.hpp>

#include "geometry/OpenSCADGenerator.hpp"

using namespace std;
using namespace gripperz::parametrization;
using namespace gripperz::geometry;
using namespace gripperz::geometry::loaders;

BOOST_AUTO_TEST_CASE(ShouldConstructProperGenerator) {
    ParametrizedMeshGeneratorLoader::Ptr loader = new ParametrizedMeshGeneratorLoader();
    ParametrizedMeshGenerator::Ptr generator = loader->load("../data/test/parametrized_mesh_generator.xml");

    OpenSCADGenerator::Ptr scad = dynamic_cast<OpenSCADGenerator*> (generator.get());
    BOOST_CHECK(scad != NULL);
    BOOST_CHECK(scad->getScriptFilename() == "../data/scad/prismatic_cutout.scad");

    Parametrization::Ptr p = scad->getParametrization();

    BOOST_CHECK_CLOSE(p->getParameter("length"), 0.2, 1e-6);
    BOOST_CHECK_CLOSE(p->getParameter("width"), 0.01, 1e-6);
}

BOOST_AUTO_TEST_CASE(ShouldSaveAndLoadGenerator) {
    ParametrizedMeshGeneratorLoader::Ptr loader = new ParametrizedMeshGeneratorLoader();
    ParametrizedMeshGenerator::Ptr generator = loader->load("../data/test/parametrized_mesh_generator.xml");
    OpenSCADGenerator::Ptr scad = dynamic_cast<OpenSCADGenerator*> (generator.get());
    
    loader->save("pmg.xml", generator);
    ParametrizedMeshGenerator::Ptr g = loader->load("pmg.xml");
    
    BOOST_CHECK(dynamic_cast<OpenSCADGenerator*> (g.get()) != NULL);
}