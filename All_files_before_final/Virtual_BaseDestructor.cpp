#include <iostream>
using namespace std;

class Base {
public:
    Base() { cout << "Base constructor\n"; }
    virtual ~Base() { cout << "Base destructor\n"; }// Virtual destructor->Ensures proper dynamic dispatch (Maintains order)
};

class Derived : public Base {
public:
    Derived() { cout << "Derived constructor\n"; }
    ~Derived() { cout << "Derived destructor\n"; }
};

int main() {
    Base* b = new Derived();  // Create a Derived object but assign it to a Base pointer
    delete b;  // Delete the object via Base pointer -> Only Base Destructor called

    return 0;
}