#include <iostream>
#include <string>
using namespace std;

class AutonomousUnit {
protected:
    string unitID;
    bool operationalStatus;
public:
    AutonomousUnit(string id = "", bool status = true) 
        : unitID(id), operationalStatus(status) {}
    virtual ~AutonomousUnit() {}
    string getUnitID() const { return unitID; }
    bool getOperationalStatus() const { return operationalStatus; }
    void setUnitID(string id) { unitID = id; }
    void setOperationalStatus(bool status) { operationalStatus = status; }

    virtual void executeMission() = 0;

    virtual string getUnitType() const = 0;

    virtual void diagnosticCheck() const {
        cout << "Unit ID: " << unitID << endl;
        cout << "Operational Status: " << (operationalStatus ? "Online" : "Offline") << endl;
    }
};

class VehicleUnit : public AutonomousUnit {
protected:
    int maxSpeed;
    float fuelLevel;
    int currentLocationX;
    int currentLocationY;
public:
    VehicleUnit(string id = "", bool status = true, int m = 0, float f = 0.0, int x = 0, int y = 0) 
        : AutonomousUnit(id, status), maxSpeed(m), fuelLevel(f), currentLocationX(x), currentLocationY(y) {}
    
    virtual ~VehicleUnit() {
        cout << "Vehicle going dark." << endl;
    }

    void setSpeed(int m) { maxSpeed = m; }
    void setFuel(float f) { fuelLevel = f; }
    void setLocationX(int x) { currentLocationX = x; }
    void setLocationY(int y) { currentLocationY = y; }
    
    int getSpeed() const { return maxSpeed; }
    float getFuel() const { return fuelLevel; }
    int getLocationX() const { return currentLocationX; }
    int getLocationY() const { return currentLocationY; }

    void move() {
        fuelLevel--;
        cout << "Moving to [" << currentLocationX << "," << currentLocationY 
             << "] at speed [" << maxSpeed << "]. Fuel remaining: [" 
             << fuelLevel << "]." << endl;
    }
    void executeMission() override {
        cout << "Performing vehicle-specific mission tasks." << endl;
    }

    string getUnitType() const override {
        return "Ground Vehicle";
    }
};

class CameraUnit : public AutonomousUnit {
protected:
    bool nightVisionMode;
    int storageCapacity;
    string recordingQuality;
public:
    CameraUnit(string id = "", bool status = true, bool nightVision = false, 
               int storage = 500, string quality = "HD") 
        : AutonomousUnit(id, status), nightVisionMode(nightVision), 
          storageCapacity(storage), recordingQuality(quality) {}
    
    virtual ~CameraUnit() {
        cout << "Camera going dark." << endl;
    }

    bool getNightVisionMode() const { return nightVisionMode; }
    int getStorageCapacity() const { return storageCapacity; }
    string getRecordingQuality() const { return recordingQuality; }
    
    void setNightVisionMode(bool mode) { nightVisionMode = mode; }
    void setStorageCapacity(int capacity) { storageCapacity = capacity; }
    void setRecordingQuality(string quality) { recordingQuality = quality; }
    void recordFootage() {
        storageCapacity--;
        cout << "Recording... Night vision is [" 
             << (nightVisionMode ? "On" : "Off") 
             << "]. Storage remaining: [" << storageCapacity << " GB]." << endl;
    }

    void executeMission() override {
        cout << "Performing camera-specific mission tasks." << endl;
    }

    string getUnitType() const override {
        return "Surveillance Unit";
    }
};

class WeaponArm {
private:
    int ammoCount;
    int damage;
    string weaponType; 
public:
    WeaponArm(int ammo = 10, int dmg = 25, string type = "Kinetic") 
        : ammoCount(ammo), damage(dmg), weaponType(type) {}
    
    ~WeaponArm() {
        cout << "Weapon going dark." << endl;
    }

    int getAmmoCount() const { return ammoCount; }
    int getDamage() const { return damage; }
    string getWeaponType() const { return weaponType; }
    
    void setAmmoCount(int ammo) { ammoCount = ammo; }
    void setDamage(int dmg) { damage = dmg; }
    void setWeaponType(string type) { weaponType = type; }

    void fire() {
        if (ammoCount > 0) {
            ammoCount--;
            cout << "Bang! Dealt [" << damage << "] damage. " 
                 << "Ammo remaining: [" << ammoCount << "]." << endl;
        } else {
            cout << "Click... Out of ammo." << endl;
        }
    }

    void reload(int amount) {
        ammoCount += amount;
        cout << "Reloaded " << amount << " rounds. Total ammo: [" 
             << ammoCount << "]." << endl;
    }

    string getWeaponInfo() const {
        return "Weapon Type: " + weaponType + ", Damage: " + to_string(damage) + 
               ", Ammo: " + to_string(ammoCount);
    }
};

class MissionObjective {
public:
    virtual void planExecution() = 0;  
    virtual void execute() = 0;           
    virtual string getMissionType() const = 0;  
    virtual int difficultyLevel() const = 0;    

    virtual ~MissionObjective() {}
};

class Reconnaissance : public MissionObjective {
private:
    string areaToScan;
    int intelPriority;
public:
    Reconnaissance(string area = "", int priority = 1) 
        : areaToScan(area), intelPriority(priority) {}

    string getAreaToScan() const { return areaToScan; }
    int getIntelPriority() const { return intelPriority; }
    void setAreaToScan(string area) { areaToScan = area; }

    void planExecution() override {
        cout << "Planning reconnaissance of [" << areaToScan 
             << "]. Priority: [" << intelPriority << "]." << endl;
    }
    void execute() override {
        cout << "Executing stealth surveillance protocol." << endl;
    }

    string getMissionType() const override {
        return "Reconnaissance";
    }

    int difficultyLevel() const override {
        return intelPriority; 
    }
};

class DefenseMission : public MissionObjective {
private:
    string perimeterToGuard;
    int threatLevel;
public:
    DefenseMission(string perimeter = "", int threat = 1) 
        : perimeterToGuard(perimeter), threatLevel(threat) {}

    string getPerimeterToGuard() const { return perimeterToGuard; }
    int getThreatLevel() const { return threatLevel; }
    void setPerimeterToGuard(string perimeter) { perimeterToGuard = perimeter; }
    void setThreatLevel(int threat) { threatLevel = threat; }

    void planExecution() override {
        cout << "Establishing defense at [" << perimeterToGuard 
             << "]. Threat: [" << threatLevel << "]." << endl;
    }

    void execute() override {
        cout << "Activating defensive countermeasures." << endl;
    }

    string getMissionType() const override {
        return "Defense";
    }

    int difficultyLevel() const override {
        return threatLevel; 
    }
};

class PatrolBot : public VehicleUnit, public CameraUnit {
private:
    string modelNumber;
    WeaponArm weapon;
    MissionObjective** assignedMissions;
    int missionCount;
    
public:
    PatrolBot(int speed, float fuel, bool nightVision, int storage, 
              string model, int ammo, int damage, int maxMissions = 10) 
        : VehicleUnit("VEH-" + model, true, speed, fuel, 0, 0),
          CameraUnit("CAM-" + model, true, nightVision, storage, "4K"),
          modelNumber(model),
          weapon(ammo, damage, "Energy"),
          missionCount(0) {
        assignedMissions = new MissionObjective*[maxMissions];
        for (int i = 0; i < maxMissions; i++) {
            assignedMissions[i] = nullptr;
        }
    }
    ~PatrolBot() {
        cout << "[" << modelNumber << "] going dark." << endl;
        delete[] assignedMissions;
    }

    void executeMission() override {
        VehicleUnit::executeMission();
        CameraUnit::executeMission();
    }

    string getUnitType() const override {
        return "Patrol Bot";
    }

    void patrolArea() {
        cout << "\n    Patrolling Area    " << endl;
        VehicleUnit::move();
        CameraUnit::recordFootage();
    }

    void engageTarget() {
        cout << "\n    Engaging Target    " << endl;
        weapon.fire();
    }
    
    void statusReport() {
        cout << "    Status Report for [" << modelNumber << "]" << endl;
        
        cout << "\nVehicle Status:" << endl;
        cout << "  Max Speed: " << maxSpeed << " km/h" << endl;
        cout << "  Fuel Level: " << fuelLevel << " units" << endl;
        cout << "  Current Location: [" << currentLocationX << "," << currentLocationY << "]" << endl;
        
        cout << "\nCamera Status:" << endl;
        cout << "  Night Vision: " << (nightVisionMode ? "Enabled" : "Disabled") << endl;
        cout << "  Storage Capacity: " << storageCapacity << " GB" << endl;
        cout << "  Recording Quality: " << recordingQuality << endl;
        
        cout << "\nWeapon Status:" << endl;
        cout << "  " << weapon.getWeaponInfo() << endl;
        
        cout << "\nMission Status:" << endl;
        cout << "  Assigned Missions: " << missionCount << endl;
    }

    void reloadWeapon(int amount) {
        weapon.reload(amount);
    }
    void assignMission(MissionObjective* newMission) {
        assignedMissions[missionCount] = newMission;
        missionCount++;
        cout << "Mission assigned to [" << modelNumber << "]. Total missions: " 
             << missionCount << endl;
    }

    void executeMissions() {
        cout << "  Executing Missions for [" << modelNumber << "]" << endl;
        
        for (int i = 0; i < missionCount; i++) {
            if (assignedMissions[i] != nullptr) {
                cout << "\nMission " << (i + 1) << " - Type: " 
                     << assignedMissions[i]->getMissionType() << endl;
                assignedMissions[i]->planExecution();
                assignedMissions[i]->execute();
            }
        }
        cout << "\nAll missions executed." << endl;
    }
    
    virtual string getCapabilityReport() const {
        string report = "Patrol Bot Capabilities:\n";
        report += "- Ground Vehicle Operations\n";
        report += "- Surveillance and Reconnaissance\n";
        report += "- Combat Engagement\n";
        report += "- Multi-Mission Support (" + to_string(missionCount) + " missions assigned)\n";
        return report;
    }
};
int main() {
    cout << "   AUTONOMOUS PATROL BOT SYSTEM   " << endl;
    
    PatrolBot bot(60, 150.0f, true, 1000, "PB-X1030", 5, 30);

    bot.statusReport();

    bot.patrolArea();
    bot.patrolArea();
    
    cout << "\n    Engaging Target Multiple Times    " << endl;

    for (int i = 0; i < 7; i++) {
        bot.engageTarget();
    }
    
    cout << "\n   Reloading Weapon    " << endl;
    bot.reloadWeapon(10);

    bot.statusReport();

    cout << "    ASSIGNING MISSIONS TO PATROL BOT   " << endl;
    
    Reconnaissance recon1("Northern Sector", 8);
    Reconnaissance recon2("Eastern Perimeter", 5);
    DefenseMission defense1("Main Base", 7);
    DefenseMission defense2("Supply Depot", 6);

    bot.assignMission(&recon1);
    bot.assignMission(&defense1);
    bot.assignMission(&recon2);
    bot.assignMission(&defense2);
    bot.executeMissions();

    cout << "  CAPABILITY REPORT " << endl;
    cout << bot.getCapabilityReport() << endl;
    bot.statusReport();

    cout << "  SYSTEM SHUTDOWN SEQUENCE" << endl;

    
    return 0;
}

