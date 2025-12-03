#include <iostream>
using namespace std;
int main() {
    int a, b, c, d;
    char x, y, z;
    cout << "Enter first fraction: ";
    cin >> a >> x >> b;
    cout << "Enter second freaction: ";
    cin >> c >> y >> d;
    cout << (a * d) + (b * c) << "//" << (b * d);
}