#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ParametrizationTranslatorTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <parametrization/VectorParametrizationTranslator.hpp>
#include <parametrization/MultiParametrizationTranslator.hpp>

#include "parametrization/Parametrization.hpp"

using namespace std;
using namespace gripperz::parametrization;

BOOST_AUTO_TEST_CASE(ShouldCreateNewParametrization) {
    vector<string> paramNames{"length", "width", "depth"};

    VectorParametrizationTranslator::Ptr t = new VectorParametrizationTranslator(paramNames);
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
    VectorParametrizationTranslator::Ptr t = new VectorParametrizationTranslator(paramNames);
    
    VectorParametrizationTranslator::ValueVector v = t->parametrizationToVector(p);
    
    vector<double> expected{3, 1, 2};
    BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(ShouldControlMultipleParameters) {
    Parametrization::Ptr p = new Parametrization(Parametrization::ParameterNameList{"a", "b", "c"});
    typedef MultiParametrizationTranslator::Slot Slot;
    ParametrizationTranslator::Ptr t = new MultiParametrizationTranslator({Slot("a"), Slot{"b", "c"}});
    
    vector<double> values{0.1, 0.2};
    t->applyVectorToParametrization(p, values);
    
    BOOST_CHECK_CLOSE(p->getParameter("a"), 0.1, 1e-6);
    BOOST_CHECK_CLOSE(p->getParameter("b"), 0.2, 1e-6);
    BOOST_CHECK_CLOSE(p->getParameter("c"), 0.2, 1e-6);
    
    vector<double> v = t->parametrizationToVector(p);
    BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), values.begin(), values.end());
}