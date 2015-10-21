#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "BeamTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <parametrization/Parametrization.hpp>

using namespace std;
using namespace gripperz::parametrization;

BOOST_AUTO_TEST_CASE(Test1) {
    Parametrization::Ptr p0 = new Parametrization();
    
    BOOST_CHECK(p0->getParameterList().size() == 0);
    BOOST_CHECK_THROW(p0->getParameter("length"), out_of_range);
    BOOST_CHECK_THROW(p0->setParameter("length", 0.0), out_of_range);
    
    Parametrization::Ptr p1 = new Parametrization({"length"});
    vector<string> expected_names{"length"};
    vector<string> names = p1->getParameterNameList();
    BOOST_CHECK_EQUAL_COLLECTIONS(names.begin(), names.end(), expected_names.begin(), expected_names.end());
    BOOST_CHECK(p1->getParameter("length") == 0.0);
    p1->setParameter("length", 1.0);
    BOOST_CHECK(p1->getParameter("length") == 1.0);
}