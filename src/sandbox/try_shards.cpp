#include <iostream>
#include <string>
#include <rw/common/Ptr.hpp>
#include "shards.hpp"

using namespace std;
using namespace rw::common;
using namespace gripperz::shards;

class Person : public ShardMaker<Person> {
public:
    typedef rw::common::Ptr<Person> Ptr;

    Person() {
    }

    void setName(const string& name) {
        _name = name;
    }
    
    string getName() const { return _name; }
    

    virtual Shard doMakeShard(ShardContainer& container) const {
        string name = container.getShard("string").cast<string>();
        
        Person::Ptr p = ownedPtr(new Person());
        p->setName(name);
        
        return Shard("PersonPtr", "id", p);
    }
    
private:
    string _name;
};

int main(int argc, char* argv[]) {
    ShardContainer container;
    
    container.addShard(Shard("string", "name", string("adam")));
    container.addShard(Person::makeShard(container));
    
    Person::Ptr p = container.getShard("PersonPtr").cast<Person::Ptr>();
    cout << p->getName() << endl;

    return 0;
}

