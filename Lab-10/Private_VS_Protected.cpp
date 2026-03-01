#include <iostream>
using namespace std;

class Base {
public:
    void show() { cout << "Base show" << endl; }
};

class Derived : protected Base {  // Protected inheritance
public:
    void callShow() { show(); }  // Accessible within Derived and further derived classes
};

class FurtherDerived : public Derived {  // Inheriting from Derived
public:
    void callShow() { show(); }  // Works in FurtherDerived because it's protected in Derived
};

int main() {
    Derived d;
    d.callShow(); // Print "Base show"
   // d.show(); // Error: show() is 'protected', so, can't access outside class
    FurtherDerived fd;
    fd.callShow();  // This works because `show()` is accessible within FurtherDerived
    //fd.show();  // Error: can't access `show()` directly, it's protected in `Derived`
}