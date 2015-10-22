#include <iostream>
#include <gripperz.hpp>
#include <boost/any.hpp>

using namespace std;
USE_ROBWORK_NAMESPACE
        using namespace robwork;
USE_GRIPPERZ_NAMESPACE
        using namespace gripperz;
using namespace boost;

class A {
public:
    typedef rw::common::Ptr<A> Ptr;
    
    void foo() {
        cout << "Hi!" << endl;
    }
};

class B {
public:
    typedef rw::common::Ptr<B> Ptr;
    
    void bar() {
        cout << "Bye!" << endl;
    }
};

int main(int argc, char* argv[]) {
    int a = 5;
    any a_star = &a;
    Ptr<any> a_ptr = &a_star;
    
    int* b = any_cast<int*>(*a_ptr);
    cout << *b << endl;
    
    /* beans */
    map<string, any> beans;
    
    any bean1 = ownedPtr(new A());
    any bean2 = ownedPtr(new B());
    
    beans["bean1"] = bean1;
    beans["bean2"] = bean2;
    
    B::Ptr bean = any_cast<B::Ptr>(beans["bean2"]);
    bean->bar();

    return 0;
}


