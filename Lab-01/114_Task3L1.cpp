#include <iostream>
using namespace std;
typedef struct fraction {
    int num;
    int den;
}frac;
int main() {
    frac a, b;
    char op ;
    cout << "Enter first fraction: ";
    cin >> a.num >> op >> a.den;
    cout << "Enter second freaction: ";
    cin >> b.num >> op >> b.den;
    cout << "Sum = " << (a.num * b.den) + (a.den * b.num) << '/' << (a.den * b.den);
}