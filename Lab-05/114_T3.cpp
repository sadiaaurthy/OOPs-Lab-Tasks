#include <iostream>
#include <string>
#include <cmath>
using namespace std;
class StockPortFolio {
private:
    string ownerName;
    double totalValue;
    int riskVolatility;
    int stockCount;
    double CalcRisk() const {
        double vol = static_cast<double>(riskVolatility);
        double val = totalValue;
        int cnt = stockCount;
        double exponent = (-val / 100000.0);
        double term1 = pow((vol * vol * val/ log(cnt + 1)), 0.6);
        double term2 = (vol * sqrt(val)) / pow(cnt, 0.3);
        double term3 = 0.4 * (vol / (1 + exp(exponent)));
        return term1 + term2 + term3;
    }
public:
    StockPortFolio() {
        ownerName = "";
        totalValue = 0.0;
        riskVolatility = 1;
        stockCount = 0;
    }
    StockPortFolio(string name, double total, int riskVol, int cnt) {
        setName(name);
        setTotalVal(total);
        setRiskVol(riskVol);
        setStockCnt(cnt);
    }
    void setName(string name) {ownerName = name;}
    void setTotalVal(double total) {totalValue = total;}
    void setRiskVol(int riskVol) {riskVolatility = riskVol;}
    void setStockCnt(int cnt) {stockCount = cnt;}

    string getName() const {return ownerName;}
    double getTotalVal() const {return totalValue;}
    int getRiskVol() const {return riskVolatility;}
    int getStockCnt() const {return stockCount;}

    StockPortFolio compareSafety(const StockPortFolio& port) const {
        double thisRisk = CalcRisk();
        double portRisk = port.CalcRisk();
        StockPortFolio safer;

        if (thisRisk < portRisk) {
            safer.setName(getName());
            safer.setTotalVal(getTotalVal());
            safer.setRiskVol(getRiskVol());
            safer.setStockCnt(getStockCnt());
        }
        else if (portRisk < thisRisk) {
            safer = port;
        }
        else {
            safer.setName(getName());
            safer.setTotalVal(getTotalVal());
            safer.setRiskVol(getRiskVol());
            safer.setStockCnt(getStockCnt());
        }
        return safer;
    }
    void DisplayDetails() const {
        cout << "Owner Name:      " << getName() << endl
             << "Total Value:     $" << getTotalVal() << endl
             << "Risk Volatility: " << getRiskVol() << "/10" << endl
             << "StockCount:      "<< getStockCnt() << endl
             << "Risk Score:      " << CalcRisk() << endl;
    }
};
int main() {
    const int SIZE = 5;
    StockPortFolio port[SIZE];
    string name;
    double value;
    int volatility;
    int count;
    cout << "Enter details for 5 portfolios:\n\n";
    for (int i = 0; i < SIZE; i++) {
        cout << "Portfolio " << i << ":\n";
        cout << "Owner name: ";
        cin >> name;
        cout << "Total value: ";
        cin >> value;
        cout << "Risk volatility (1-10): ";
        cin >> volatility;
        cout << "Stock count: ";
        cin >> count;
        port[i].setName(name);  
        port[i].setTotalVal(value);
        port[i].setRiskVol(volatility);
        port[i].setStockCnt(count);
        cout << endl;
    }
    StockPortFolio safer = port[0];
    for (int i = 1; i < SIZE; i++) {
        safer = safer.compareSafety(port[i]);/*Object BEFORE . is CALLING object*/
    }
    cout << "\nSafest Portfolio Found:\n";
    safer.DisplayDetails();
    return 0;
}
