#include <iostream>
#include <string>
using namespace std;

class VehicleUnit {
private:
    int maxSpeed; float fuelLevel;
public:
    VehicleUnit(int msp = 0, float fl = 0.0) : maxSpeed(msp), fuelLevel(fl) {}

    void setMaxSpeed(int msp) {maxSpeed = msp;}
    void setFuelLevel(float fl) {fuelLevel = fl;}

    int getMaxSpeed() const {return maxSpeed;}
    float getFuelLevel() const {return fuelLevel;}

    ~VehicleUnit() {
        cout << "Vehicle going dark.\n";
    }

    void move() {
        fuelLevel--;
        cout << "Moving at Speed : " << getMaxSpeed() << " ";
        cout << "Fuel remaining : " << getFuelLevel() << '.' << endl;
    }
};

class CameraUnit {
private:
    bool nightVisionMode; int storageCapacity;
public:
    CameraUnit(bool nVM = false, int sCap = 0) : nightVisionMode(nVM), storageCapacity(sCap) {}

    void setVision(bool nVM) {nightVisionMode = nVM;}
    void setSCap(int sCap) {storageCapacity = sCap;}

    bool getVision() const {return nightVisionMode;}
    int getSCap() const {return storageCapacity;}

    ~CameraUnit() {
        cout << "Camera going dark\n";
    }
    void recordFootage() {
        storageCapacity--;
        cout << "Recording..... Night Vision is " << getVision()
             << ". Storage remaining : " << getSCap() << " GB.\n";
    }

};

class WeaponArm {
private:
    int ammoCount, damage;
public:
    WeaponArm(int ammo = 0, int dam = 0) : ammoCount(ammo), damage(dam) {}

    void setAmmoCnt(int ammo) {ammoCount = ammo;}
    void setDamage(int dam) {damage = dam;}

    int getAmmoCnt() const {return ammoCount;}
    int getDamage() const {return damage;}

    ~WeaponArm() {
        cout << "Weapon going dark.\n";
    }
    void Fire() {
        if (getAmmoCnt() > 0) {
            ammoCount--;
            cout << "Bang! Dealt " << getDamage() << " damage.\n";
        } else {
            cout << "Click..... Out of ammo.\n";
        }
    }
    void Reload(int amount) {
        ammoCount += amount;
        cout << "After reload ammunitions are available : " << getAmmoCnt() << endl;
    }
};

class PatrolBot : protected VehicleUnit, protected CameraUnit {
private:
    string modelNumber;
    WeaponArm armObj;
public:
    PatrolBot(int msp = 0, float fl = 0.0, bool nVM = false, int sCap = 0, string model = "", int ammo = 0, int dam = 0) 
    : VehicleUnit(msp, fl), CameraUnit(nVM, sCap), modelNumber(model), armObj(ammo, dam) {} 

    void setModel(string model) {modelNumber = model;}

    string getModel() const {return modelNumber;}

    ~PatrolBot() {
        cout << "Model Number " << getModel() << " going dark.\n";
    }

    void patrolArea() {
        cout << "    --- Patrolling Area ---   \n";
        move();
        recordFootage();
    }

    void engageTarget() {
        cout << "    --- Engaging Target ---   \n";
        armObj.Fire();
    }

    void statusReport() {
        cout << "\n    --- STATUS REPORT ---    \n\n";

        cout << "    --- VEHICLE STATUS ---    \n";
        cout << "Max Speed: " << getMaxSpeed() << " Km/h"<< endl;
        cout << "Fuel Level: " << getFuelLevel() << " unit" << endl;

        cout << "    --- CAMERA UNIT STATUS ---    \n";
        cout << "Night Vision Mode: " << (getVision()? "ON": "OFF") << endl;
        cout << "Storage Capacity: " << getSCap() << " GB" << endl;

        cout << "    --- WEAPON ARM STATUS ---    \n";
        cout << "Current Ammunitions available : " << armObj.getAmmoCnt() << endl;
        cout << "Damage : " << armObj.getDamage() << endl;
    }  
    
    void Reload(int amount){
        armObj.Reload(amount);
    }
};

int main() {
    PatrolBot bot(60, 150.0f, true, 1000, "PB-X1000", 5, 30);
    bot.statusReport();
    bot.patrolArea();
    
    for (int i = 0; i < 7; i++) {
        bot.engageTarget();
    }
    bot.Reload(10) ;
    for (int i = 0; i < 7; i++) {
        bot.engageTarget();
    }
    bot.statusReport();
    for (int i = 0; i < 7; i++) {
        bot.engageTarget();
    }
    bot.patrolArea();
    bot.statusReport();
    return 0;
}