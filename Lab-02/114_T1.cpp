#include <iostream>
using namespace std;
class BatchMeter {
private: 
    int total;
    int pulseSize;
public: 
    BatchMeter() {
        total = 0;
        pulseSize = 1;
    }
    void setPulseSize(int s) {
        if (s > 0) pulseSize = s;
        else {
            cout << "Invalid pulse; must be > 0" << endl;
        }
    }
    int getTotal() {
        return total;
    }
    void addPulse() {
        total += pulseSize;
    }
    void clear() {
        total = 0;
    }
};
int main() {
   BatchMeter m;
   int size;
   cin >> size;
   m.setPulseSize(size);
   m.addPulse();
   cout << m.getTotal() << endl;
   m.addPulse();
   m.addPulse();
   cout << m.getTotal() << endl;
   m.setPulseSize(-2);
   m.clear();
   cout << m.getTotal() << endl;
   return 0;
}
