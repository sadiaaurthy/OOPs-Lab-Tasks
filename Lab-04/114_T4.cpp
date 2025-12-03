#include <iostream>
using namespace std;

class FarlinkSatellite {
private:
    float altitude;
    static int orbitCount;  

public:
    
    FarlinkSatellite() {
        altitude = 430.2;
        orbitCount++;
        cout << "Satellite deployed. Orbit count: [" << orbitCount << "]" << endl;
    }

    ~FarlinkSatellite() {
        orbitCount--;
        cout << "Satellite de-orbited and burned up. Orbit count: [" << orbitCount << "]" << endl;
    }


    void adjustAltitude(float change) {
        altitude += change;

        if (altitude < 230.4) {
            cout << "Altitude too low! Satellite in re-entry zone!" << endl;
            altitude = 230.4; 
        }
        cout << "Satellite altitude adjusted to: " << altitude << " km" << endl;
    }

    static int getOrbitCount() {
        return orbitCount;
    }
};

int FarlinkSatellite::orbitCount = 0;

int main() {

    FarlinkSatellite satellite1; 
    satellite1.adjustAltitude(50); 
    satellite1.adjustAltitude(-100);
    cout << "Current number of active satellites: " << FarlinkSatellite::getOrbitCount() << endl;

    FarlinkSatellite satellite2;
    FarlinkSatellite satellite3;
    cout << "Current number of active satellites: " << FarlinkSatellite::getOrbitCount() << endl;

    {
        FarlinkSatellite satellite4;
        cout << "Current number of active satellites: " << FarlinkSatellite::getOrbitCount() << endl;
    }

    cout << "Current number of active satellites: " << FarlinkSatellite::getOrbitCount() << endl;

    return 0;
}
