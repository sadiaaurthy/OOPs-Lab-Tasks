#include <iostream>
#include <string>
using namespace std;

class AutonomousUnit {
private:
    string unitID;
    bool operationalStatus;
public:
    AutonomousUnit(string uid = "", bool opState = false) 
     : unitID(uid), operationalStatus(opState) {}

    void setUnitType(string uid) {unitID = uid;}
    void setOpStatus(bool opState) {operationalStatus = opState;}

    virtual string getUnitType() const = 0;
    bool getOpStatus() const {return operationalStatus;}

    virtual void executeMission() const = 0;

    void diagnosticCheck() const {
        cout << "Autonomous Unit Type: " << getUnitType() << endl;
        cout << "Operation Status: " << getOpStatus() << endl;
    }

    ~AutonomousUnit() {
        cout << "Destructor has been called\n";
    }
};

class VehicleUnit : public AutonomousUnit {
private:
    int maxSpeed, currLocationX, currLocationY; float fuelLevel;
public:
    VehicleUnit(int msp = 0, float fl = 0.0, int currX = 0, int currY = 0) 
     : maxSpeed(msp), fuelLevel(fl), currLocationX(currX), currLocationY(currY) {}

    void setMaxSpeed(int msp) {maxSpeed = msp;}
    void setFuelLevel(float fl) {fuelLevel = fl;}
    void setCurrLocX(int currX) {currLocationX = currX;}
    void setCurrLocY(int currY) {currLocationY = currY;}

    int getMaxSpeed() const {return maxSpeed;}
    float getFuelLevel() const {return fuelLevel;}
    int getX() const {return currLocationX;}
    int getY() const {return currLocationY;}

    void move() {
        fuelLevel--;
        cout << "Moving to [" << getX()<< ',' << getY() << "] at speed " << getMaxSpeed() << ".\n";
        cout << "Fuel remaining: [" << getFuelLevel() << "].\n";
    }

    void executeMission() const override {
        cout << "Perform vehicle-specific mission tasks.\n";
    }

    string getUnitType() const override {return "Ground Vehicle\n";}

    ~VehicleUnit() {
        cout << "Vehicle going dark.....\n";
    }

};

class CameraUnit :  public AutonomousUnit {
private:
    bool nightVisionMode; int storageCapacity; string recordingQuality;
public:
    CameraUnit(bool nVM = false, int sCap = 0, string recQ = "")
     : nightVisionMode(nVM), storageCapacity(sCap), recordingQuality(recQ) {}

    void setVision(bool nVM) {nightVisionMode = nVM;}
    void setSCap(int sCap) {storageCapacity = sCap;}
    void setRecQ(string recQ) {recordingQuality = recQ;}

    bool getVision() const {return nightVisionMode;}
    int getSCap() const {return storageCapacity;}
    string getRecQ() const {return recordingQuality;}

    ~CameraUnit() {
        cout << "Camera going dark\n";
    }
    void recordFootage() {
        storageCapacity--;
        cout << "Recording..... Night Vision is " << (getVision()? "ON":"OFF")
             << ". Storage remaining : " << getSCap() << " GB.\n";
    }

    void executeMission() const override {
        cout << "Performing camera-specific mission tasks.\n";
    }

    string getUnitType() const override {return "Surveillance Unit\n";}
};

class WeaponArm {
private:
    int ammoCount, damage;
    string weaponType;
public:
    WeaponArm(int ammo = 0, int dam = 0, string wType = "")
     : ammoCount(ammo), damage(dam), weaponType(wType) {}

    void setAmmoCnt(int ammo) {ammoCount = ammo;}
    void setDamage(int dam) {damage = dam;}
    void setWeaponType(string wType) {weaponType = wType;}

    int getAmmoCnt() const {return ammoCount;}
    int getDamage() const {return damage;}
    string getWeaponType() const {return weaponType;}

    ~WeaponArm() {
        cout << "Weapon going dark....\n";
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

class MissionObjective {
public:
    virtual void planExecution() = 0;
    virtual void execute() = 0;
    virtual string getMissionType() const = 0;
    virtual int difficultyLevel() const = 0;
    virtual ~MissionObjective() {};
};

class ReconMission : public MissionObjective { // Defining EACH & EVERY pure virtual func of abstract class in the derived class is MUST
private:
    string areaToScan; int intelPriority;
public:
    ReconMission(string area = "", int prior = 0)
     : areaToScan(area), intelPriority(prior) {}

    void setScanArea(string area) {areaToScan = area;}
    void setPriority(int prior) {intelPriority = prior;}

    string getScanArea() const {return areaToScan;}
    int getPriority() const {return intelPriority;}

    void planExecution() override {
        cout << "Planning reconnaissance of " << getScanArea() << " .\n";
        cout << "Priority: " << getPriority() << endl; 
    }

    void execute() override {
        cout << "Executing stealth surveillance protocol.\n";
    }

    string getMissionType() const override {return "Reconnaisance\n";}
    int difficultyLevel() const override {return getPriority();}
};

class DefenseMission : public MissionObjective {
private:
    string perimeterToGuard;
    int threatLevel;
public:
    DefenseMission(string per = "", int tLev = 0)
     : perimeterToGuard(per), threatLevel(tLev) {}
    
    void setPeriGuard(string per) {perimeterToGuard = per;}
    void setThreatLev(int tLev) {threatLevel = tLev;}

    string getPeriGuard() const {return perimeterToGuard;}
    int getThreatLev() const {return threatLevel;}

    void planExecution() override {
        cout << "Establishing defense at " << getPeriGuard() << " .\n";
        cout << "Threat: " << getThreatLev() << ".\n";
    }

    void execute() override {
        cout << "Activating defensive countermeasures.\n";
    }

    string getMissionType() const override {return "Defense.\n";}

    int difficultyLevel() const override {return getThreatLev();}
};

class PatrolBot : public VehicleUnit, public CameraUnit {
private:
    string modelNumber;
    WeaponArm armObj;
    MissionObjective** assignedMissionsArr;
    int missionCount;
public:
    PatrolBot(int msp = 0, float fl = 0.0, int currX = 0, int currY = 0, bool nVM = false, int sCap = 0, string recQ = "",
      string model = "", int ammo = 0, int dam = 0, string wType = "", int missCnt = 0) 
    : VehicleUnit(msp, fl, currX, currY), CameraUnit(nVM, sCap, recQ), modelNumber(model), armObj(ammo, dam, wType), missionCount(missCnt) {
        assignedMissionsArr = new MissionObjective*[missCnt];
        for (int i = 0; i < missCnt; i++) {
            assignedMissionsArr[i] = NULL;
        } 
    } 

    void setModel(string model) {modelNumber = model;}
    void setMissionCnt(int missCnt) {missionCount = missCnt;}

    string getModel() const {return modelNumber;}
    int getMissionCnt() const {return missionCount;}

    ~PatrolBot() {
        cout << "Model Number " << getModel() << " going dark....\n";
        delete[] assignedMissionsArr;
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

    void assignMissionFunc(MissionObjective* newMission) {
        assignedMissionsArr[getMissionCnt()] = newMission;
        missionCount++;
        cout << "Mission assigned to [" << getModel() << "].\n";
        cout << "Total missions: " << getMissionCnt() << endl;
    }

    void executeAllMissions() {
        for (int i = 0; i < getMissionCnt(); i++) {
            assignedMissionsArr[i]->planExecution();
            assignedMissionsArr[i]->execute();
        }
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
    PatrolBot bot(60, 150.0f, 1000, 1000, true, 256, "4K", "Hober Mallo - X9876", 5, 30, "Ruger 10/22", 5);

    bot.statusReport();
    
    bot.patrolArea();
    
    for (int i = 0; i < 7; i++) {
        bot.engageTarget();
    }

    bot.Reload(10) ;

    for (int i = 0; i < 7; i++) {
        bot.engageTarget();
    }

    bot.Reload(8);

    bot.statusReport();

    ReconMission r1("North Pole", 10), r2("West Perimeter", 15);
    DefenseMission d1("Main Base", 5), d2("Supply Depot", 4);

    bot.assignMissionFunc(&r1);
    bot.assignMissionFunc(&r2);
    bot.assignMissionFunc(&d1);
    bot.assignMissionFunc(&d2);

    bot.executeAllMissions();

    cout << "\n==============================================\n";
    cout << "   --- CAPABILITY REPORT ---    \n";
    cout << "\n==============================================\n";

    cout << bot.getCapabilityReport() << endl;

    bot.statusReport();

    return 0;
}