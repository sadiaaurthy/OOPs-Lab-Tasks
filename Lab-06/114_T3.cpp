#include <iostream>
using namespace std;

class Elevator {
private:
    int currentFloor;
    const int maxFloor;

public:
    Elevator() : currentFloor(0), maxFloor(100) {
        cout << "Elevator created at Ground Floor. Max Floor: " << maxFloor << endl;
    }

    Elevator(int initial, int max) : currentFloor(initial), maxFloor(max) {
        if (currentFloor < 0) currentFloor = 0;
        if (currentFloor > maxFloor) currentFloor = maxFloor;
        cout << "Elevator created at Floor " << currentFloor << ". Max Floor: " << maxFloor << endl;
    }
    ~Elevator() {
        cout << "Elevator destroyed. Was at Floor: " << currentFloor << endl;
    }

    int getCurrentFloor() const {
        return currentFloor;
    }

    int getMaxFloor() const {
        return maxFloor;
    }
    void setCurrentFloor(int floor) {
        if (floor < 0) {
            cout << "Error: Floor cannot be less than 0. Setting to Ground Floor." << endl;
            currentFloor = 0;
        } else if (floor > maxFloor) {
            cout << "Error: Floor cannot exceed " << maxFloor << ". Setting to Top Floor." << endl;
            currentFloor = maxFloor;
        } else {
            currentFloor = floor;
        }
    }
    
    void operator--() {
        if (currentFloor == 0) {
            cout << "Ground Floor Reached" << endl;
        } else {
            currentFloor--;
            cout << "Moved down to Floor: " << currentFloor << endl;
        }
    }

    Elevator operator--(int) {
        Elevator temp(currentFloor, maxFloor);
        if (currentFloor > 0) {
            currentFloor--;
        }
        return temp;
    }

    friend istream& operator>>(istream& in, Elevator& e) {
        int floor;
        cout << "Enter floor number: ";
        in >> floor;

        if (floor < 0) {
            cout << "Clamping to Ground Floor (0)" << endl;
            floor = 0;
        } else if (floor > e.maxFloor) {
            cout << "Clamping to Top Floor (" << e.maxFloor << ")" << endl;
            floor = e.maxFloor;
        }
        
        e.currentFloor = floor;
        return in;
    }

    friend ostream& operator<<(ostream& out, const Elevator& e) {
        out << "Elevator is currently at Floor: " << e.currentFloor;
        return out;
    }
};

int main() {
    Elevator e1;
    cout << endl;

    cout << "Security setting initial position:" << endl;
    cin >> e1;
    cout << endl;

    cout << "Status check:" << endl;
    cout << e1 << endl << endl;

    cout << "Moving down (prefix --):" << endl;
    --e1;
    cout << endl;

    cout << "Logging previous state (postfix --):" << endl;
    Elevator prev = e1--;
    cout << "Elevator prev = ";
    cout << prev << endl;
    cout << "Current: ";
    cout << e1 << endl << endl;

    cout << "Testing ground floor limit:" << endl;
    e1.setCurrentFloor(0);
    --e1;
    cout << endl;

    cout << "Creating elevator with max floor 50:" << endl;
    Elevator e2(25, 50);
    cout << e2 << endl << endl;

    cout << "Testing input clamping (try entering 150):" << endl;
    cin >> e2;
    cout << e2 << endl;
    
    cout << endl << "=== Program ending ===" << endl;
    
    return 0;
}