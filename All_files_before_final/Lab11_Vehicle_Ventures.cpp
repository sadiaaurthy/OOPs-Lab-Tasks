#include <iostream>
#include <string>
using namespace std;

class IVehicle {
public:
    virtual void start() = 0;
    virtual void stop() = 0; 
    virtual ~IVehicle() {};
};

class IGPSVehicle {
public:
    virtual void enableGPS(string ssid) const = 0;
    virtual void disableGPS() = 0;
    virtual ~IGPSVehicle() {};
};

class Vehicle : public IVehicle {
private:
    string vehicleName;
public:
    Vehicle(string name = "") : vehicleName(name) {}
    
    void setName(string name) {vehicleName = name;}

    string getName() const {return vehicleName;}

    void start() override {cout << "Vehicle Started.\n";}

    void stop() override {cout << "Vehicle Stopped.\n";}

    void showVehicle() {cout << "Vehicle Name: [" << getName() << "]\n";}
};

class ConnectedVehicle : public IVehicle, public IGPSVehicle {
private:
    string vehicleName;
    int fuelLevel;
public:
    ConnectedVehicle(string name = "", int fLev = 0)
     : vehicleName(name), fuelLevel(fLev) {}

    void setName(string name) {vehicleName = name;}
    void setFuelLev(int fLev) {fuelLevel = fLev;}

    string getName() const {return vehicleName;}
    int getFuelLev() const {return fuelLevel;}

    void start() override {cout << "Connected Vehicle started.\n";}
    void stop() override {cout << "Connected Vehicle stopped.\n";}

    void enableGPS(string ssid) const {cout << "GPS enabled with " << ssid << endl;}
    void disableGPS() {cout << "GPS disabled\n";}

    void refuel(int amount) {fuelLevel += amount;}
    void checkfuel() {cout << "Current Fuel Level: " << getFuelLev() << " L" << endl;}
}; 

class AutonomousCar: public ConnectedVehicle {
private: 
    string carID;
public:
    AutonomousCar(string name = "", int fLev = 0, string cid = "")
     : ConnectedVehicle(name, fLev), carID(cid) {}

    void setCarID(string cid) {carID = cid;}

    string getCarID() const {return carID;}

    void start() override {cout << "Autonomous Car " << getCarID() << " is ready to drive.\n";}

    void navigateTo(string destination) {cout << "Driving to " << destination << ".\n";}

    void checkDistance(float dist) {cout << "Total distance is " << dist << " km.\n";}

    void selfPark()  {cout << "Car " << getCarID() << " is parking itself.\n";}
};

void testVehicle(IVehicle* v) {
        v->start();
        v->stop();
}
        
int main() {
    AutonomousCar car("Rolls-Royce", 500, "MX - 7005");

    car.refuel(500);
    car.start();
    car.enableGPS("gps-3.2");
    car.navigateTo("Los Angeles");
    car.checkDistance(945.949);
    car.refuel(-300);
    car.selfPark();
    car.disableGPS();
    car.stop();

    cout << "\n   --- Test The Vehicles Now ---   \n";

    Vehicle v("Tesla\n");
    testVehicle(&v);

    ConnectedVehicle cv("Lamborghini", 450);
    testVehicle(&cv);

    AutonomousCar ac("Range Rover", 600, "RR - 90995");
    testVehicle(&ac);

    return 0;
}