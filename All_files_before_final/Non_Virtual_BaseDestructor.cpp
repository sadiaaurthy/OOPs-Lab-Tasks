#include <iostream>
using namespace std;

class Base {
public:
    Base() { cout << "Base constructor\n"; }
    ~Base() { cout << "Base destructor\n"; }
};

class Derived : public Base {
public:
    Derived() { cout << "Derived constructor\n"; }
    ~Derived() { cout << "Derived destructor\n"; }
};

int main() {
    Base* b = new Derived();  // Create a Derived object but assign it to a Base pointer
    delete b;  // Delete the object via Base pointer -> b is Base(non-virtual so performs static binding)
    return 0;
}