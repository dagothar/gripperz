#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "GripperQualityLoaderTest"

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <models/loaders/GripperQualityLoader.hpp>

using namespace std;
using namespace gripperz::models;
using namespace gripperz::models::loaders;

BOOST_AUTO_TEST_CASE(ShouldLoadQuality) {
    GripperQualityLoader::Ptr loader = new GripperQualityLoader();
    GripperQuality::Ptr quality = loader->load("../data/test/quality.xml");
    
    BOOST_CHECK_CLOSE(quality->getIndex("success"), 1.0, 1e-6);
    BOOST_CHECK_CLOSE(quality->getIndex("coverage"), 0.5, 1e-6);
    BOOST_CHECK_CLOSE(quality->getIndex("alignment"), 0.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(ShouldSaveQuality) {
    GripperQualityLoader::Ptr loader = new GripperQualityLoader();
    GripperQuality::Ptr quality = loader->load("../data/test/quality.xml");
    
    loader->save("q.xml", quality);
    GripperQuality::Ptr q = loader->load("q.xml");
    
    BOOST_FOREACH(QualityIndex i, quality->getIndices()) {
        BOOST_CHECK(q->getIndex(i.first) == i.second);
    }
}