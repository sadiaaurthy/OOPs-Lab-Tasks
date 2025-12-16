#include <iostream>
#include <cmath>
#include <string>
using namespace std;

class StockPortfolio {
private:
    string ownerName;
    double totalValue;
    int riskVolatility;
    int stockCount;

    double calculateRiskScore() const {
        double volatility = static_cast<double>(riskVolatility);
        double value = totalValue;
        int count = stockCount;
    
        double term1 = pow((volatility * volatility * value) / log(count + 1), 0.6);

        double term2 = pow((volatility * sqrt(value)) / count, 0.3);

        double exponent = -value / 100000.0;
        double term3 = 0.4 * (volatility / (1 + exp(exponent)));
        
        return term1 + term2 + term3;
    }

public:
    StockPortfolio() {
        ownerName = "";
        totalValue = 0.0;
        riskVolatility = 1;
        stockCount = 0;
    }

    StockPortfolio(string name, double value, int volatility, int count) {
        ownerName = name;
        totalValue = value;
        riskVolatility = volatility;
        stockCount = count;
    }
    
 
    string getOwnerName() const {
        return ownerName;
    }
    
    double getTotalValue() const {
        return totalValue;
    }
    
    int getRiskVolatility() const {
        return riskVolatility;
    }
    
    int getStockCount() const {
        return stockCount;
    }

    void setOwnerName(string name) {
        ownerName = name;
    }
    
    void setTotalValue(double value) {
        totalValue = value;
    }
    
    void setRiskVolatility(int volatility) {
        riskVolatility = volatility;
    }
    
    void setStockCount(int count) {
        stockCount = count;
    }

    StockPortfolio compareSafety(const StockPortfolio& other) const {
        double thisRisk = calculateRiskScore();
        double otherRisk = other.calculateRiskScore();
        
        cout << "Comparing portfolios:" << endl;
        cout << "  " << ownerName << " - Risk Score: " << thisRisk << endl;
        cout << "  " << other.ownerName << " - Risk Score: " << otherRisk << endl;
        
     
        if (thisRisk < otherRisk) {
            cout << "  Winner: " << ownerName << " (Lower risk)" << endl << endl;
            return *this;
        } else if (otherRisk < thisRisk) {
            cout << "  Winner: " << other.ownerName << " (Lower risk)" << endl << endl;
            return other;
        } else {
            cout << "  Winner: " << ownerName << " (Equal risk)" << endl << endl;
            return *this;
        }

    }

    void displayDetails() const {
        cout << "Owner Name:       " << ownerName << endl;
        cout << "Total Value:      $" << totalValue << endl;
        cout << "Risk Volatility:  " << riskVolatility << "/10" << endl;
        cout << "Stock Count:      " << stockCount << endl;
        cout << "Risk Score:       " << calculateRiskScore() << endl;
    }
};

int main() {

    StockPortfolio portfolios[5] ;
    StockPortfolio safer = portfolios[0];
    
    for (int i = 1; i < 5; i++) {
        cout << "Round " << i << ":" << endl;
        safer = safer.compareSafety(portfolios[i]);
    }
    
    safer.displayDetails();
    
    return 0;
}