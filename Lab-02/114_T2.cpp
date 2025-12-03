#include <iostream>
#include <cmath>
using namespace std;

class GridNavigator {
private:
    double homeX, homeY;     
    double currentX, currentY;

public:
    GridNavigator() {
        homeX = 0.0;
        homeY = 0.0;
        currentX = 0.0;
        currentY = 0.0;
    }

    void setHome(double x, double y) {
        homeX = x;
        homeY = y;
        currentX = x;
        currentY = y;
    }

    void moveBy(double dx, double dy) {
        currentX += dx;
        currentY += dy;
    }

    void resetToHome() {
        currentX = homeX;
        currentY = homeY;
    }

    double getX() { 
        return currentX;
    }
    double getY() { 
        return currentY; 
    }

    double distanceTo(double x, double y) {
        double dx = x - currentX;
        double dy = y - currentY;
        return sqrt(dx * dx + dy * dy);
    }

    void printLocation() {
        cout << "Position: (" << currentX << ", " << currentY << ")" << endl;
    }
};

int main() {
    GridNavigator nav;
    nav.printLocation();

    nav.moveBy(3.5, -2.1);
    nav.printLocation();

    cout << "Distance to (0,0): " << nav.distanceTo(0, 0) << endl;

    nav.setHome(10, 10);
    nav.printLocation();

    nav.moveBy(5, 5);
    nav.printLocation();

    nav.resetToHome();
    nav.printLocation();

    return 0;
}