#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ShardContainerTest"

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/foreach.hpp>
#include <shards/ShardContainer.hpp>

#include "shards/ShardExceptions.hpp"

using namespace std;
using namespace gripperz::shards;

class Person {
public:

    Person(const string& name) : _name(name) {
    }

    string getName() const {
        return _name;
    }

private:
    string _name;
};

BOOST_AUTO_TEST_CASE(ShouldStoreAndReturnShards) {
    ShardContainer container;
    
    vector<Shard> shards{
      Shard("string", "s1", string("hello")),  
      Shard("string", "s2", string("world")),  
      Shard("string", "s3", string("bye"))  
    };
    
    BOOST_FOREACH (Shard& shard, shards) {
        container.addShard(shard);
    }
    
    vector<ShardContainer::ShardPair> res = container.getShards();
    BOOST_CHECK(res.size() == 3);
    BOOST_CHECK(res[0].first == "string");
    BOOST_CHECK(res[0].second.getId() == "s1");
    BOOST_CHECK(res[0].second.cast<string>() == "hello");
}

BOOST_AUTO_TEST_CASE(ShouldReturnProperClasses) {

    ShardContainer container;

    Shard s1("int", "int1", 1);
    container.addShard(s1);
    
    Shard s2("string", "string1", string("aaa"));
    container.addShard(s2);

    BOOST_CHECK(container.getShard("int").cast<int>() == 1);
    BOOST_CHECK(container.getShard("string").cast<string>() == "aaa");
}

BOOST_AUTO_TEST_CASE(ShouldThrowUnresolvedAndAmbiguous) {
    ShardContainer container;

    Shard s1("int", "int1", 1);
    container.addShard(s1);
    
    Shard s2("string", "string1", string("aaa"));
    container.addShard(s2);
    
    Shard s3("string", "string2", string("bbb"));
    container.addShard(s3);
    
    BOOST_CHECK_THROW(container.getShard("double"), unresolved_shard_exception);
    BOOST_CHECK_THROW(container.getShard("string"), ambiguous_shard_exception);
    BOOST_CHECK(container.getShard("string", "string2").cast<string>() == "bbb");
}