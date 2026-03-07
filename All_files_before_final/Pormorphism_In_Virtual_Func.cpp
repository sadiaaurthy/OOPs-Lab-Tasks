#include <iostream>
using namespace std;

// Base class
class Shape {
public:
    // Virtual function allows polymorphism
    virtual void draw() const {
        cout << "Drawing a generic shape." << endl;
    }

    // Virtual destructor to ensure proper cleanup of derived objects
    virtual ~Shape() {
        cout << "Shape destructor called." << endl;
    }
};

// Derived class 1
class Circle : public Shape {
public:
    void draw() const override {
        cout << "Drawing a circle." << endl;
    }

    ~Circle() override {
        cout << "Circle destructor called." << endl;
    }
};

// Derived class 2
class Rectangle : public Shape {
public:
    void draw() const override {
        cout << "Drawing a rectangle." << endl;
    }

    ~Rectangle() override {
        cout << "Rectangle destructor called." << endl;
    }
};

int main() {
    // // Create a base class pointer and assign derived class objects to it
    // Shape* shape1 = new Circle();
    // Shape* shape2 = new Rectangle();

    // // Call draw() method, but it will use the derived class' implementation
    // shape1->draw();  // Outputs: "Drawing a circle."
    // shape2->draw();  // Outputs: "Drawing a rectangle."

    // // Clean up memory and call destructors
    // delete shape1;  // Outputs: "Circle destructor called." and "Shape destructor called."
    // delete shape2;  // Outputs: "Rectangle destructor called." and "Shape destructor called."

    Circle c1;
    Rectangle r1; // Object're created in Stack so destructor of 'Rectangle' called first

    //r1.draw();
    c1.draw();
    r1.draw();

    return 0;
}