#include <iostream>
#include <string>
using namespace std;

class IExplorer {
public:
    virtual void observe() const = 0;
    virtual void report() const = 0;
};

class IDataCollector {
public:
    virtual void collectSample(string sampleType) const = 0;
    virtual void storeData() const = 0;
};

class BasicExplorer : public IExplorer {
private: 
    string name;
public:
    BasicExplorer(string str = "") : name(str) {}

    void setName(string str) {name = str;}
    string getName() const {return name;}

    void observe() const override {cout << "BasicExplorer " << getName() << " is observing the area.\n";}
    void report() const override {cout << "BasicExplorer " << getName() << " reports general findings.\n";}

    void introduce() {cout << "Hello, I am " << getName() << ", a basic explorer.\n";}
};

class FieldScientist : public IExplorer , public IDataCollector {
private:
    string name; 
    int fieldExperience;
public:
    FieldScientist(string str = "", int yr = 5)
     : name(str), fieldExperience(yr) {}

    void setName(string str) {name = str;}
    void setExp(int yr) {fieldExperience = yr;}

    string getName() const {return name;}
    int getExp() const {return fieldExperience;}

    void observe() const override {cout << "FieldScientist " << getName() << " is observing with " << getExp() << " years of experience.\n";}
    void report() const override {cout <<  "FieldScientist " << getName() << " reports general findings.\n";}

    void collectSample(string sampleType) const {cout << "Collecting " << sampleType << " sample.\n";}
    void storeData() const {cout << "FieldScientist is strong data\n";}

    void analyzeSample(string sampleType) {cout << "Analyzing " << sampleType << " sample.\n";}
    void showExperience() {cout << "Experience: " << getExp() << " years\n";}
};

class EcoDrone : public FieldScientist {
private:
    string droneID;
public:
    EcoDrone(string str = "", int yr = 5, string did = "")
     : FieldScientist(str, yr), droneID(did) {}
    
    void setDroneID(string did) {droneID = did;}

    string getDroneID() const {return droneID;}

    void observe() const override {cout << "Drone " << getDroneID() << " is scanning the environment from above.\n";}
    void mapTerrain() {cout << "Mapping terrain using drone " << getDroneID() << ".\n";}
    void trackAnimal(string species) {cout << "Drone " << getDroneID() << " is tracking " << species << ".\n";}
    void performMaintenance() {cout << "Drone " << getDroneID() << " performing maintenance check.\n";}
};
void testReport(IExplorer* e) {
    e->observe();
    e->report();
}

int main() {
    EcoDrone ecd("Claudia", 12, "DRONE-9088");
    ecd.observe();
    ecd.collectSample("Water");
    ecd.analyzeSample("Soil");
    ecd.mapTerrain();
    ecd.trackAnimal("Tiger");
    ecd.performMaintenance() ;

    cout << "\n      --- Test Report ---    \n\n";

    BasicExplorer be("Sizuka") ;
    testReport(&be);

    FieldScientist fs("Nobita", 7);
    testReport(&fs);
    
    EcoDrone ec("Jayan", 8, "DRONE-MX1982");
    testReport(&ec);
}