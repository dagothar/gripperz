#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "ShardTest"

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <shards/Shard.hpp>

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

BOOST_AUTO_TEST_CASE(InitializationTest) {
    Person person("Adam");
    Shard personShard("Person", "p1", person);

    Person result = personShard.cast<Person>();

    BOOST_CHECK(person.getName() == result.getName());
}

BOOST_AUTO_TEST_CASE(ShouldGetAndSetId) {
    Person person("Adam");
    Shard personShard("Person", "person1", person);
    
    BOOST_CHECK(personShard.getQualifier() == "person1");

    personShard.Qualifier("person2");
    BOOST_CHECK(personShard.getQualifier() == "person2");
}