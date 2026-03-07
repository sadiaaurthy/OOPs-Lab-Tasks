#include <iostream>
#include <stdexcept> // For exceptions
using namespace std;

class CakeOrder {
public:
    CakeOrder(int ingredientCount) {
        if (ingredientCount <= 0) {
            // Throw an exception if no ingredients are provided
            throw std::invalid_argument("Error: No ingredients provided!");
        }
        cout << "Cake order placed with " << ingredientCount << " ingredients." << endl;
    }
};

int main() {
    try { // ERROR occur in the try{} block
        CakeOrder order1(0); // This try{} block will throw an exception
    } catch (const std::invalid_argument& e) { // catches the excp thrown from the mem func
        cout << "Exception caught: " << e.what() << endl; // Handle error gracefully, e.what()-> what has been thrown from member function
    }

    return 0;
}