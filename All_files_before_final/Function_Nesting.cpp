#include <iostream>
#include <stdexcept>
using namespace std;

// Lower-level function: Simulates checking balance
void checkBalance(double balance, double withdrawalAmount) {
    if (balance < withdrawalAmount) {
        // If insufficient balance, throw exception
        throw std::runtime_error("Insufficient funds in account!");
    }
    cout << "Balance is sufficient." << endl;
}

// Intermediate-level function: Simulates a bank operation
void bankOperation(double balance, double withdrawalAmount) {
    try {
        // Call to the lower-level function that checks balance
        checkBalance(balance, withdrawalAmount);
    }
    catch (const std::runtime_error& e) {
        // Rethrow exception with extra context (banking system)
        throw std::runtime_error("Bank error: " + string(e.what()));
    }
}

// Top-level function: ATM operation (this is where the try block is)
void atmOperation(double balance, double withdrawalAmount) {
    try {
        // Call the bank operation which has its own try-catch
        bankOperation(balance, withdrawalAmount);
    }
    catch (const std::runtime_error& e) {
        // Catch the rethrown exception from the bank operation
        cout << "Error occurred: " << e.what() << endl;
    }
}

int main() {
    double balance = 50.0;    // Current balance
    double withdrawalAmount = 100.0; // Attempt to withdraw more than balance

    // Call the top-level ATM operation
    atmOperation(balance, withdrawalAmount);

    return 0;
}