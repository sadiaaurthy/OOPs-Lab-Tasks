#include <iostream>
#include <exception>
using namespace std;

class Base {
public:
    virtual void display() { cout << "Base display" << endl; }
};

class Derived : public Base {
public:
    void display() override { cout << "Derived display" << endl; }
};

int main() {
    Base* basePtr = new Base();
    Base* derivedPtr = new Derived();

    // Using static_cast (no runtime check)
    Derived* d1 = static_cast<Derived*>(basePtr);  // Undefined behavior: basePtr is not pointing to Derived
    d1->display();  // Undefined behavior

    // Using dynamic_cast (runtime check)
    Derived* d2 = dynamic_cast<Derived*>(basePtr);  // Will return nullptr
    if (d2) {
        d2->display();
    } else {
        cout << "Dynamic cast failed: basePtr is not pointing to Derived." << endl;
    }

    // Using dynamic_cast on references (throws std::bad_cast if failed)
    try {
        Derived& d3 = dynamic_cast<Derived&>(*basePtr);  // Throws std::bad_cast
        d3.display();
    } catch (const std::bad_cast& e) {
        cout << "Caught exception: " << e.what() << endl;
    }

    delete basePtr;
    delete derivedPtr;
    return 0;
}