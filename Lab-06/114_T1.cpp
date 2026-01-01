#include <iostream>
#include <string>
using namespace std;

class DataPack {
private:
    float dataGB;          
    string provider;       
    bool exhausted;        

public:
    
    DataPack(const string& providerName = "Unknown", float data = 0.0f)
        : provider(providerName), dataGB(0.0f), exhausted(false)
    {
        setDataGB(data);
    }

    ~DataPack() {
        cout << "Connection Terminated" << endl;
    }

    // Getters
    float getDataGB() const {
        return dataGB;
    }

    string getProvider() const {
        return provider;
    }

    bool isExhausted() const {
        return exhausted;
    }

    // Setters
    void setDataGB(float data) {
        if (data < 0) {
            dataGB = 0;
            exhausted = true;
        } else {
            dataGB = data;
            exhausted = (dataGB == 0);
        }
    }

    void setProvider(const string& providerName) {
        provider = providerName;
    }

    void setExhausted(bool status) {
        exhausted = status;
    }


    // Add two DataPack objects
    DataPack operator+(const DataPack& other) const {
        DataPack temp(provider, dataGB + other.dataGB); // Use other.getdataGB(), better choice
        return temp;
    }

    // Add extra GB to DataPack(only right side covered)
    DataPack operator+(float extraGB) const {
        DataPack temp(provider, dataGB + extraGB);
        return temp;
    }

    // Consume data (subtraction)
    DataPack operator-(float usedGB) const {
        DataPack temp(*this);
        if (usedGB >= temp.dataGB) {
            temp.dataGB = 0;
            temp.exhausted = true;
        } else {
            temp.dataGB -= usedGB;
        }
        return temp;
    }

    operator int() const {
        return static_cast<int>(dataGB * 1024); // GB → MB
    }

    void printData() const {
        cout << "Provider: " << provider << endl;
        cout << "Available Data: " << dataGB << " GB" << endl;
        cout << "Status: " << (exhausted ? "Exhausted" : "Active") << endl;

    }

    friend DataPack operator+(float extraGB, const DataPack& dp); 
    // friend function to cover left side
};

DataPack operator+(float extraGB, const DataPack& dp) {
    DataPack temp(dp.provider, dp.dataGB + extraGB);
    return temp;
}
