#include <iostream>
#include <string>
using namespace std;

class Furniture {
private:
    string material;
    int maxWeightCap;
    bool rest;
public:
    Furniture(string mat = "", int wCap = 0, bool r = false) 
    : material(mat), maxWeightCap(wCap), rest(r) {}

    void setMaterial(string mat) {material = mat;}
    void setMaxWeightCap(int wCap) {maxWeightCap = wCap;}
    void setRest(bool r) {rest = r;}

    string getMaterial() const {return material;}
    int getMaxWeightCap() const {return maxWeightCap;}
    string getRest() const {return rest? "True" : "False";}
   
    void PrintFurnitureDetails() {
        cout << "Chair is made of: " << getMaterial() << endl;
        cout << "Weight Capacity: " << getMaxWeightCap() << " Kg\n";
        if (getRest() == "True") {
            cout << "User is resting on the " << getMaterial() << " of the  the Furniture.\n";
        } else {
            cout << "Nobody is resting.\n";
        }
    }
};

class PowerDevice {
private:
    float voltageRequirement;
    bool currPowState;
public:
    PowerDevice(float volReq = 0.0, bool powState = false) 
    : voltageRequirement(volReq), currPowState(powState) {}

    void setVoltageReq(float volReq) {voltageRequirement = volReq;}
    void setCurrPowState(bool powState) {currPowState = powState;}

    float getVoltageReq() const {return voltageRequirement;}
    bool getCurrPowState() const {return currPowState;}

    void PrintPowerDeviceDetails () {
        cout << "Current Power State : " << (getCurrPowState()? "ON\n" : "OFF\n");
    }
};

class RumbleUnit{
private:
    bool vibrate;
    int intensity;
public:
    RumbleUnit(bool vib = false, int f = 0) : vibrate(vib), intensity(f) {}

    void setVibrate(bool vib) {vibrate = vib;}
    void setIntensity(int f) {intensity = f;}

    bool getVibrate() const {return vibrate;}
    int getIntensity() const {return intensity;}
    
    void PrintRumbleUnitDetails() {
        if (getVibrate()) {
            cout << "The Rumble unit is vibrating at intensity: " << getIntensity() << endl;
        } else {
            cout << "Rumble unit stopped\n";
        }
    }
};

class Tablet {
private:
    int batteryCap;
    string brand;
public:
    Tablet(int batCap = 0, string br = "") : batteryCap(batCap), brand(br) {}

    void setBatteryCap(int batCap) {batteryCap = batCap;}
    void setBrand(string br) {brand = br;}

    int getBatteryCap() const {return batteryCap;}
    string getBrand() const {return brand;}
};

class SmartStation : public Furniture, public PowerDevice {
private:
    string firmwareVersion;
    RumbleUnit ru;
    Tablet *tb;
public:
    SmartStation(string mat = "", int wCap = 0, bool r = false, float volReq = 0.0, bool powState = false, string ver = "", bool vib = false, int f = 0, int batCap = 0, string br = "")
     : Furniture(mat, wCap, r), PowerDevice(volReq, powState), firmwareVersion(ver), ru(vib, f), tb(NULL) {

        if (batCap > 0 && !br.empty()) {
            tb = new Tablet(batCap, br);
        }
    }

    ~SmartStation() {
        if (tb) delete tb;
    }
    void setFirwareVersion(string ver) {firmwareVersion = ver;}

    string getFirmwareVersion() const {return firmwareVersion;}

    void PrintFirmwareVersion() {
        cout << "Firmware Version : " << getFirmwareVersion() << endl;
    }

    void PrintSmartStationDetails() {
        PrintFurnitureDetails();
        PrintPowerDeviceDetails ();
        ru.PrintRumbleUnitDetails();
        if (tb) {
            cout << "The chair is being controlled by " << tb->getBrand() 
                 << ", Battery Capacity " << tb->getBatteryCap() << " mAh" << endl;
        } else {
            cout << "No tablet docked\n";
        }
    }
};

int main() {
    SmartStation ironThrone("Leather", 55, true, 220.0, true, "Ver-01", true, 75, 5000, "Samsung");

    ironThrone.PrintSmartStationDetails();
    ironThrone.setCurrPowState(false);
    
    cout << endl;
    SmartStation ironThrone2("Wood", 49, false, 220.0, false, "Ver-01", false, 55, 0);

    ironThrone2.PrintSmartStationDetails();
}




// Without Overhead (\n): The program quickly fills a "buffer" (a small waiting room in RAM) with data. Once the buffer is full, it sends everything to the screen at once in one big "trip".
// With Overhead (endl): You force the program to "empty the bucket" for every single line, even if it only has one word in it. This makes the hardware work much harder and more frequently. 