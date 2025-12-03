#include <iostream>
using namespace std;

class RaceCar {
private:
    const int maxSpeed;  
    double currentSpeed; 

public:
    RaceCar(int maxSpeed) : maxSpeed(maxSpeed), currentSpeed(0) {}

    ~RaceCar() {
        cout << "Car returned to garage." << endl;
    }

    void accelerate(double amount) {
        if (currentSpeed + amount > maxSpeed) {
            cout << "Velocity limit exceeded. Please consult the laws of physics." << endl;
        } else {
            currentSpeed += amount;
        }
    }

    bool isFasterThan(const RaceCar& other) const {
        return currentSpeed > other.currentSpeed;
    }

    void dashboard() const {
        cout << "Current Speed: " << currentSpeed << " km/h" << endl;
        cout << "Maximum Speed: " << maxSpeed << " km/h" << endl;
    }
};

int main() {
    int s1, s2;
    cout << "Speed of Car1: ";
    cin >> s1;
    cout << "Speed of Car2: ";
    cin >> s2;
    RaceCar car1(s1);
    RaceCar car2(s2);
    int a1, a2;
    cout << "Acceleration of Car1: ";
    cin >> a1;
    cout << "Acceleration of Car2: ";
    cin >> a2;
    car1.accelerate(a1); 
    car2.accelerate(a2); 

    cout << "Car 1 Dashboard: " << endl;
    car1.dashboard();

    cout << "Car 2 Dashboard: " << endl;
    car2.dashboard();

    if (car1.isFasterThan(car2)) {
        cout << "Car 1 is faster than Car 2." << endl;
    } else {
        cout << "Car 1 is not faster than Car 2." << endl;
    }

    return 0;
}
