#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ParametrizationTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <parametrization/Parametrization.hpp>

using namespace std;
using namespace gripperz::parametrization;

BOOST_AUTO_TEST_CASE(NullParametrization) {
    Parametrization::Ptr p = new Parametrization();
    
    BOOST_CHECK(p->getParameterList().size() == 0);
    BOOST_CHECK_THROW(p->getParameter("length"), out_of_range);
    BOOST_CHECK_THROW(p->setParameter("length", 0.0), out_of_range);
}

BOOST_AUTO_TEST_CASE(TestAddingParameters) {
    Parametrization::Ptr p = new Parametrization({"length"});
    BOOST_CHECK(p->hasParameter("length") == true);
    vector<string> expected_names{"length"};
    vector<string> names = p->getParameterNameList();
    BOOST_CHECK_EQUAL_COLLECTIONS(names.begin(), names.end(), expected_names.begin(), expected_names.end());
    BOOST_CHECK(p->getParameter("length") == 0.0);
    p->setParameter("length", 1.0);
    BOOST_CHECK(p->getParameter("length") == 1.0);
}