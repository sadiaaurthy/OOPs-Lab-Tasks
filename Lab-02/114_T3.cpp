#include <iostream>
#include <cstring>
using namespace std;

class SupplyAlert {
private:
    int currentStock;
    int threshold;
    char itemID[20];

public:
    SupplyAlert() {
        currentStock = 0;
        threshold = 5;
        itemID[0] = '\0';
    }

    void setThreshold(int t) {
        if (t > 0) {
            threshold = t;
        } else {
            cout << "Invalid threshold; must be > 0" << endl;
        }
    }

    void setItemID(const char id[]) {
        strncpy(itemID, id, 19);
        itemID[19] = '\0';
    }

    void addStock(int n) {
        if (n > 0) {
            currentStock += n;
        } else {
            cout << "Invalid stock addition" << endl;
        }
    }

    void consume(int n) {
        if (n < 0 || currentStock - n < 0) {
            cout << "Invalid consumption" << endl;
        } else {
            currentStock -= n;
        }
    }

    bool isLow() {
        return currentStock <= threshold;
    }

    void profile() {
        cout << "Item " << itemID << ": Stock " << currentStock
             << ", Threshold " << threshold << ", "
             << (isLow() ? "LOW" : "OK") << endl;
    }
};

int main() {
    SupplyAlert s;
    s.setItemID("CHIPS");
    s.addStock(10);
    s.consume(7);
    s.profile();

    return 0;
}