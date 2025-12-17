#include <iostream>
#include <string>
using namespace std;

class DataPack {
private:
    float availableDataGB;
    string providerName;
    bool exhausted;

public:
    DataPack() : availableDataGB(0.0), providerName("Unknown"), exhausted(true) {
        cout << "DataPack created with default values" << endl;
    }
    
    DataPack(const string& provider, float initialData = 0.0) 
        : providerName(provider), availableDataGB(initialData), exhausted(initialData <= 0.0) {
        if (initialData < 0.0) {
            availableDataGB = 0.0;
            exhausted = true;
            cout << "Warning: Negative data amount set to 0.0 GB" << endl;
        }
        cout << "DataPack created for " << provider << " with " << availableDataGB << " GB" << endl;
    }

    ~DataPack() {
        cout << "Connection Terminated for " << providerName << endl;
    }

    float getAvailableData() const {
        return availableDataGB;
    }
    
    string getProviderName() const {
        return providerName;
    }
    
    bool isExhausted() const {
        return exhausted;
    }

    void setAvailableData(float data) {
        if (data < 0.0) {
            cout << "Error: Cannot set negative data amount. Setting to 0.0 GB" << endl;
            availableDataGB = 0.0;
            exhausted = true;
        } else {
            availableDataGB = data;
            exhausted = (data <= 0.0);
        }
    }
    
    void setProviderName(const string& provider) {
        if (provider.empty()) {
            cout << "Error: Provider name cannot be empty" << endl;
            return;
        }
        providerName = provider;
    }
    
    void setExhausted(bool status) {
        exhausted = status;
    }

    DataPack operator+(const DataPack& other) const {
        DataPack result(providerName + "+" + other.providerName, 
                       availableDataGB + other.availableDataGB);
        return result;
    }

    DataPack operator+(float additionalGB) const {
        if (additionalGB < 0.0) {
            cout << "Warning: Cannot add negative data. Ignoring operation." << endl;
            DataPack result(providerName, availableDataGB);
            return result;
        }
        DataPack result(providerName, availableDataGB + additionalGB);
        return result;
    }

    friend DataPack operator+(float additionalGB, const DataPack& pack) {
        return pack + additionalGB;
    }

    DataPack operator-(float consumptionGB) const {
        if (consumptionGB < 0.0) {
            cout << "Warning: Cannot consume negative data. Ignoring operation." << endl;
            DataPack result(providerName, availableDataGB);
            return result;
        }
        
        DataPack result(providerName);
        
        if (consumptionGB >= availableDataGB) {
            result.availableDataGB = 0.0;
            result.exhausted = true;
            cout << "Warning: Data exhausted! Consumption (" << consumptionGB 
                 << " GB) exceeds available data (" << availableDataGB << " GB)" << endl;
        } else {
            result.availableDataGB = availableDataGB - consumptionGB;
            result.exhausted = false;
        }
        
        return result;
    }

    operator int() const {
        return static_cast<int>(availableDataGB * 1024);
    }

    void printData() const {
     
        cout << "Provider: " << providerName << endl;
        cout << "Available Data: " << availableDataGB << " GB" << endl;
        cout << "Status: " << (exhausted ? "EXHAUSTED" : "ACTIVE") << endl;
       
    }

    void printDataInMB() const {
        int mb = static_cast<int>(availableDataGB * 1024);
        cout << "Available Data: " << mb << " MB" << endl;
    }
};

void demonstratePassByReference(const DataPack& pack) {
    cout << "\n--- Demonstrating const pass by reference ---" << endl;
    pack.printData();
}

int main() {
    cout << "===== Dormitory Internet Data Manager =====" << endl << endl;

    DataPack d1("Campus WiFi", 5.0);
    d1.printData();

    cout << "\n--- Adding 2.5 GB ---" << endl;
    d1 = d1 + 2.5;
    d1.printData();

    cout << "\n--- Consuming 1.2 GB (streaming lecture) ---" << endl;
    d1 = d1 - 1.2;
    d1.printData();

    cout << "\n--- Converting to Megabytes ---" << endl;
    int mb = d1;
    cout << "Remaining data in MB: " << mb << " MB" << endl;

    cout << "\n--- Attempting to consume 10 GB (Netflix binge) ---" << endl;
    d1 = d1 - 10.0;
    d1.printData();

    if (d1.isExhausted()) {
        cout << "\n*** WARNING: Data pack exhausted! Time to buy more data! ***" << endl;
    }

    cout << "\n--- Creating and combining data packs ---" << endl;
    DataPack d2("Mobile Data", 3.0);
    DataPack d3 = d1 + d2;
    d3.printData();
    
    demonstratePassByReference(d3);
    
    cout << "\n--- Testing error handling ---" << endl;
    DataPack d4("Test Provider", -2.0); 
    d4.printData();
    
    cout << "\n--- Program ending, destructors will be called ---" << endl;
    return 0;
}