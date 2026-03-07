#include <iostream>
#include <string>
using namespace std;
class IExplorer {
public:
    virtual void observe() = 0;
    virtual void report() = 0;
    virtual ~IExplorer() {}
};

class IDataCollector {
public: 
    virtual void collectSample(string sampleType) = 0;
    virtual void storeData() = 0;
    virtual ~IDataCollector() {}
};

class BasicExplorer : public IExplorer {
protected: 
    string name;
public: 
    BasicExplorer(string n) : name(n) {}
    void observe() override {
        cout << "Explorere " << name << " is observing the area.\n";
    }
    void report() override {
        cout << "Explorer " << name << "reports general findings.\n";
    }
    void Introduce() {
        cout << "Hello, I'm " << name << ", basic explorere.\n";
    }
};

class FieldScientist : public IExplorer, public IDataCollector {
protected:
    string name;
    int fieldExperience;
public:
    FieldScientist(string name) : name(name), fieldExperience(5) {}
    void observe() override { cout << "FieldScientist " << name << " is observing with " << fieldExperience << " years of experience." << endl; }
    void report() override { cout << "FieldScientist " << name << " reports detailed scientific findings." << endl; }
    void collectSample(string sampleType) override { cout << "Collecting " << sampleType << " sample." << endl; }
    void storeData() override { cout << "Storing collected data securely." << endl; }
    void analyzeSample(string sampleType) { cout << "Analyzing " << sampleType << " sample." << endl; }
    void showExperience() { cout << "Experience: " << fieldExperience << " years" << endl; }
};

class EcoDrone : public FieldScientist {
private:
    string droneID;
public:
    EcoDrone(string name, string id) : FieldScientist(name), droneID(id) {}

    void observe() override {
        FieldScientist::observe();
        cout << "Drone " << droneID << " is scanning the environment from above." << endl;
    }

    void mapTerrain() { cout << "Mapping terrain using drone " << droneID << "." << endl; }
    void trackAnimal(string species) { cout << "Drone " << droneID << " is tracking " << species << "." << endl; }
    void performMaintenance() { cout << "Drone " << droneID << " performing maintenance check." << endl; }
};

void testExplorer(IExplorer *e) {
    e->observe();
    e->report();
}

int main() {
    cout << "     EcoDrone           " << endl;
    EcoDrone drone("Dr. Maya", "DRONE-X1");

    drone.observe();
    drone.collectSample("water");
    drone.analyzeSample("water");
    drone.mapTerrain();
    drone.trackAnimal("tiger");
    drone.performMaintenance();
    drone.report();

    cout << "\n      testExplorer()       " << endl;

    cout << "\n-- BasicExplorer --" << endl;
    BasicExplorer be("Alex");
    testExplorer(&be);

    cout << "\n-- FieldScientist --" << endl;
    FieldScientist fs("Dr. Sara");
    testExplorer(&fs);

    cout << "\n-- EcoDrone --" << endl;
    testExplorer(&drone);

    return 0;
}













