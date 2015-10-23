#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ParametrizationLoaderTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <parametrization/Parametrization.hpp>
#include <parametrization/loaders/ParametrizationLoader.hpp>

using namespace std;
using namespace gripperz::parametrization;
using namespace gripperz::parametrization::loaders;

BOOST_AUTO_TEST_CASE(ShouldLoadParametrization) {
    ParametrizationLoader::Ptr loader = new ParametrizationLoader();
    Parametrization::Ptr p = loader->load("../data/test/parametrization.xml");
    
    Parametrization::ParameterNameList expected{"pi", "length", "zero"};
    Parametrization::ParameterNameList names = p->getParameterNameList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), names.begin(), names.end());
    
    BOOST_CHECK_CLOSE(p->getParameter("pi"), 3.14, 1e-6);
    BOOST_CHECK_CLOSE(p->getParameter("length"), 1.0, 1e-6);
    BOOST_CHECK_CLOSE(p->getParameter("zero"), 0.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(ShouldSaveParametrization) {
    ParametrizationLoader::Ptr loader = new ParametrizationLoader();
    Parametrization::Ptr p1 = loader->load("../data/test/parametrization.xml");
    
    loader->save("p.xml", p1);
    Parametrization::Ptr p2 = loader->load("p.xml");
    
    Parametrization::ParameterNameList pl1 = p1->getParameterNameList();
    Parametrization::ParameterNameList pl2 = p2->getParameterNameList();
    
    BOOST_CHECK_EQUAL_COLLECTIONS(pl1.begin(), pl1.end(), pl2.begin(), pl2.end());
}