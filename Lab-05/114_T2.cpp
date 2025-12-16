#include <iostream>
#include <string>
using namespace std;

class CyberImplant {
private:
    string modelName;
    string bodyPart;
    int powerConsumption;
    double firmwareVersion;

public:
    CyberImplant() {
        modelName = "";
        bodyPart = "";
        powerConsumption = 0;
        firmwareVersion = 1.0;
    }

    CyberImplant(string model, string part, int power, double version) {
        modelName = model;
        setBodyPart(part);
        setPowerConsumption(power);
        firmwareVersion = version;
    }

    string getModelName() const {
        return modelName;
    }

    string getBodyPart() const {
        return bodyPart;
    }

    int getPowerConsumption() const {
        return powerConsumption;
    }

    double getFirmwareVersion() const {
        return firmwareVersion;
    }

    void setModelName(string model) {
        modelName = model;
    }

    void setBodyPart(string part) {
        if (part == "Arm" || part == "Leg" || part == "Eye" || part == "Heart") {
            bodyPart = part;
        } else {
            bodyPart = "Auxiliary";
        }
    }

    void setPowerConsumption(int power) {
        if (power < 0) {
            powerConsumption = 0;
        } else if (power > 1000) {
            powerConsumption = 1000;
        } else {
            powerConsumption = power;
        }
    }

    void setFirmwareVersion(double version) {
        firmwareVersion = version;
    }

    void displayStats() const {
        cout << "Model: " << modelName 
             << " | Part: " << bodyPart 
             << " | Power: " << powerConsumption << "W"
             << " | Firmware: v" << firmwareVersion << endl;
    }
};

void upgradeFirmware(CyberImplant &implant) {
    implant.setFirmwareVersion(implant.getFirmwareVersion() + 0.1);
    implant.setPowerConsumption(implant.getPowerConsumption() - 5);
}

int main() {
    CyberImplant implants[5];  
    string m, b;
    int p;
    double ver;
    for (int i = 0; i < 2; i++) {
        cout << "ModelName: ";
        cin.ignore();
        getline(cin, m);
        implants[i].setModelName(m);
        cout << "bodyPart: ";
        getline(cin, b);
        implants[i].setBodyPart(b);
        cout << "powerConsumption: ";
        cin >> p;
        implants[i].setPowerConsumption(p);
        cout << "firmwareVersion: ";
        cin >> ver;
        implants[i].setFirmwareVersion(ver);
        cout << endl;
    }
    cout << endl;
    cout << "  UPDATED IMPLANT STATS  " << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Implant " << i + 1 << " - ";
        implants[i].displayStats();
    }

    return 0;
}