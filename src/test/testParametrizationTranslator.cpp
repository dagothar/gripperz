#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ParametrizationTranslatorTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <parametrization/ParametrizationTranslator.hpp>

using namespace std;
using namespace gripperz::parametrization;

BOOST_AUTO_TEST_CASE(ShouldCreateNewParametrization) {
    vector<string> paramNames{"length", "width", "depth"};

    ParametrizationTranslator::Ptr t = new ParametrizationTranslator(paramNames);
    Parametrization::Ptr p = t->vectorToParametrization({0, 1, 2});

    BOOST_CHECK(p != NULL);
    BOOST_CHECK(p->hasParameter("length"));
    BOOST_CHECK(p->hasParameter("width"));
    BOOST_CHECK(p->hasParameter("depth"));
    BOOST_CHECK(p->getParameter("length") == 0);
    BOOST_CHECK(p->getParameter("width") == 1);
    BOOST_CHECK(p->getParameter("depth") == 2);
}

BOOST_AUTO_TEST_CASE(ShouldExtractParameterValues) {
    Parametrization::Ptr p = new Parametrization(Parametrization::ParameterList{{"a", 1}, {"b", 2}, {"c", 3}});
    
    BOOST_CHECK(p->getParameter("a") == 1);
    BOOST_CHECK(p->getParameter("b") == 2);
    BOOST_CHECK(p->getParameter("c") == 3);
    
    vector<string> paramNames{"c", "a", "b"};
    ParametrizationTranslator::Ptr t = new ParametrizationTranslator(paramNames);
    
    ParametrizationTranslator::ValueVector v = t->parametrizationToVector(p);
    
    vector<double> expected{3, 1, 2};
    BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), expected.begin(), expected.end());
}