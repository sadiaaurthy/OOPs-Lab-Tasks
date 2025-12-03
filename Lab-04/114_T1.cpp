#include <iostream>
using namespace std;
class CoffeeMaker {
private: 
    int currWater;
    int currBeans;
    int capWater;
public: 
    CoffeeMaker(int maxWater){
        capWater = maxWater;
        currWater = 0;
        currBeans = 0;
    }
    ~CoffeeMaker() {
        cout << "Shutting down system. Cleaning internal pipes." << endl;
    }
    void addResources(int water, int beans) {
        if (water < 0 || beans < 0) {
            cout << "Error: Negative values are not allowed." << endl;
            return ;
        }
        if (currWater + water > capWater) {
            cout << "Exceeds water capacity." << endl;
            return ;
        } 
        currWater += water;
        currBeans += beans;
    }
    void brewCup(int sl) {
        if (sl < 1 || sl > 3) {
            cout << "Strength level must be between 1 to 3." << endl;
            return;
        }
        int reqWater = 150;
        int reqBeans = sl * 7;
        if (currWater < reqWater) {
            cout << "Refill needed for water." << endl;
            return;
        }
        if (currBeans < reqBeans) {
            cout << "Refill needed for beans." << endl;
            return;
        }
    }
    void reportStatus() {
        int waterPercentage = (currWater * 100) / capWater;
        cout << "Beans : " << currBeans << " grams" << endl;
        cout << "Water : " << waterPercentage << "%" << endl;
    }
};
int main() {
    int maxWater;
    cout << "maxWater: ";
    cin >> maxWater;
    cout << endl;
    CoffeeMaker coffeeMaker(maxWater);
    cout << "water: " << endl;
    int water, beans;
    cin >> water ;
    cout << "Beans: " << endl;
    cin >> beans;
    coffeeMaker.addResources(water, beans); 
    coffeeMaker.reportStatus();
    int sl;
    cin >> sl;
    coffeeMaker.brewCup(sl); 
    coffeeMaker.reportStatus(); 

    coffeeMaker.addResources(900, 500); 
    coffeeMaker.reportStatus(); 

    return 0;
}










