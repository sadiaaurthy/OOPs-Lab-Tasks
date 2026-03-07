#include <iostream>
#include <string>
using namespace std;
class IVehicle {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~IVehicle() {}
};

class IGPSVehicle {
public:
    virtual void enableGPS(string ssid) = 0;
    virtual void disableGPS() = 0;
    virtual ~IGPSVehicle() {}
};

class Vehicle : public IVehicle {
protected:
    string vehicleName;
public:
    Vehicle(string name) : vehicleName(name) {}
    void start() override { cout << "Vehicle started" << endl; }
    void stop()  override { cout << "Vehicle stopped" << endl; }
    void showVehicle() { cout << "Vehicle name: " << vehicleName << endl; }
};
class ConnectedVehicle : public IVehicle, public IGPSVehicle {
protected:
    string vehicleName;
    int fuelLevel;
public:
    ConnectedVehicle(string name) : vehicleName(name), fuelLevel(0) {}
    void start()   override { cout << "Vehicle started" << endl; }
    void stop()    override { cout << "Vehicle stopped" << endl; }
    void enableGPS(string ssid)  override { cout << "GPS enabled with SSID: " << ssid << endl; }
    void disableGPS()            override { cout << "GPS disabled" << endl; }
    void refuel(int amount) { fuelLevel += amount; cout << "Refueled by " << amount << ". " ; checkFuel(); }
    void checkFuel() { cout << "Fuel level: " << fuelLevel << endl; }
};

class AutonomousCar : public ConnectedVehicle {
private:
    string carID;
public:
    AutonomousCar(string name, string id) : ConnectedVehicle(name), carID(id) {}

    void start() override {
        cout << "Vehicle started" << endl;
        cout << "Autonomous Car " << carID << " is ready to drive." << endl;
    }

    void navigateTo(string destination) {
        cout << "Driving to " << destination << "." << endl;
    }

    void checkDistance(float dist) {
        cout << "Total distance is " << dist << "km." << endl;
        int fuelUsed = (int)(dist * fuelLevel * 0.10);
        fuelLevel -= fuelUsed;
        cout << "Fuel used: " << fuelUsed << ". ";
        checkFuel();
    }

    void selfPark() {
        cout << "Car " << carID << " is parking itself." << endl;
    }
};

void testVehicle(IVehicle *v) {
    v->start();
    v->stop();
}

int main() {
    cout << "      AutonomousCar Demo      " << endl;
    AutonomousCar ac("Tesla Model X", "AC-001");
    ac.refuel(100);
    ac.start();
    ac.enableGPS("MyNetwork");
    ac.navigateTo("Downtown");
    ac.checkDistance(5.0f);
    ac.checkFuel();
    ac.refuel(50);
    ac.selfPark();
    ac.disableGPS();
    ac.stop();

    cout << "\n         testVehicle()           " << endl;

    cout << "\n   Vehicle   " << endl;
    Vehicle v("BasicCar");
    testVehicle(&v);

    cout << "\n   ConnectedVehicle   " << endl;
    ConnectedVehicle cv("SmartCar");
    testVehicle(&cv);

    cout << "\n   AutonomousCar   " << endl;
    AutonomousCar ac2("Waymo", "AC-002");
    testVehicle(&ac2);

    return 0;
}
