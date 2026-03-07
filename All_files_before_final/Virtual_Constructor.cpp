#include <iostream>
using namespace std;
class Base {
public:
    virtual Base* create() = 0;  // Simulating a virtual constructor(C++ doesn't support it)
};

class Derived : public Base {
public:
    Derived* create() override {
        return new Derived();
    }
};

int main() {
    Base* b = new Derived();
    Base* newObj = b->create();  // Calls Derived's create(), which returns a new Derived object
}