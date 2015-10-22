#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "BeamTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <geometry/Beam.hpp>
#include <geometry/BeamFactory.hpp>

using namespace rw::geometry;
using namespace gripperz::geometry;

BOOST_AUTO_TEST_CASE(LoadTest) {
    TriMesh::Ptr mesh = NULL;
    Beam::Ptr beam = BeamFactory::makeBeam(mesh);

    BOOST_CHECK(beam->moment(0) == 0.0);
    BOOST_CHECK(beam->moment(1) == 0.0);

    beam->addMoment(-1, 0);

    BOOST_CHECK(beam->moment(0) == -1);
    BOOST_CHECK(beam->moment(1) == -1);
    
    beam->addForce(1, 0);
    
    BOOST_CHECK(beam->moment(0) == -1);
    BOOST_CHECK(beam->moment(1) == 0);
    BOOST_CHECK(beam->moment(2) == 1);
    
    beam->addForce(-1, 1);
    
    BOOST_CHECK(beam->moment(0) == -1);
    BOOST_CHECK(beam->moment(1) == 0);
    BOOST_CHECK(beam->moment(2) == 0);
}