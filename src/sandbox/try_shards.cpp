#include <iostream>
#include <string>
#include <rw/common/Ptr.hpp>
#include "shards.hpp"

using namespace std;
using namespace rw::common;
using namespace gripperz::shards;

/*----------------------------------------------------------------------------*/

class Pet {
public:
    typedef rw::common::Ptr<Pet> Ptr;

    Pet() {
    }

    string getSpecies() const {
        return _species;
    }

    void setSpecies(const string& s) {
        _species = s;
    }

private:
    string _species;
};

namespace gripperz {
    namespace shards {

        template<>
        Shard makeShard(ShardContainer& container, const Shard::ShardQualifier& qual, const Pet::Ptr& object) {
            string species = container.getShard("string", "species").cast<string>();
            object->setSpecies(species);
            return Shard("PetPtr", "pet", object);
        }
    }
}

/*----------------------------------------------------------------------------*/

class Person : public ShardMaker<Person> {
public:
    typedef rw::common::Ptr<Person> Ptr;

    Person() {
    }

    void setName(const string& name) {
        _name = name;
    }

    string getName() const {
        return _name;
    }

    void setPet(Pet::Ptr pet) {
        _pet = pet;
    }

    Pet::Ptr getPet() const {
        return _pet;
    }

    virtual Shard doMakeShard(ShardContainer& container, const Shard::ShardQualifier& qual) const {
        string name = container.getShard("string", "name").cast<string>();
        Pet::Ptr pet = container.getShard("PetPtr", "pet").cast<Pet::Ptr>();

        Person::Ptr p = ownedPtr(new Person());
        p->setName(name);
        p->setPet(pet);

        return Shard("PersonPtr", qual, p);
    }

private:
    string _name;
    Pet::Ptr _pet;
};

/*----------------------------------------------------------------------------*/

int main(int argc, char* argv[]) {
    /* Create the container and shards */
    ShardContainer container;

    container.addShard(makeShard<string>(container, "name", "Adam"));
    container.addShard(makeShard<string>(container, "species", "cat"));
    container.addShard(makeShard<Pet::Ptr>(container, "pet", ownedPtr(new Pet())));
    container.addShard(makeShard<Person>(container, ""));

    /* Use created shards */
    Person::Ptr p = container.getShard("PersonPtr").cast<Person::Ptr>();
    cout << p->getName() << endl;
    cout << "has a " << p->getPet()->getSpecies() << endl;

    return 0;
}

