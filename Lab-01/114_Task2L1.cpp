#include <iostream>
using namespace std;
enum etype {
    laborer, secretary, manager, accountant, executive, researcher
};
int main() {
    char ch;
    cout << "Enter employee type (first letter only): ";
    cin >> ch;
    switch(ch) {
        case 'l':
        cout << "Employee type is laborer" << endl;
        break;
        case 's':
        cout << "Employee type is secretary" << endl;
        break;
        case 'm':
        cout << "Employee type is manager" << endl;
        break;
        case 'a':
        cout << "Employee type is accountant" << endl;
        break;
        case 'e':
        cout << "Employee type is executive" << endl;
        break;
        case 'r':
        cout << "Employee type is researcher" << endl;
        break;
        default:
        cout << "Invalid Input" << endl;
    }

}