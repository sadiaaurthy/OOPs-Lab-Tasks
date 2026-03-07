#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────────
// MenuItem class
// ─────────────────────────────────────────────
class MenuItem {
public:
    int id;
    string name;
    double price;

    // Constructor with sensible defaults
    MenuItem(int id = 0, const string &name = "", double price = 0.0)
        : id(id), name(name), price(price) {}

    // Destructor
    ~MenuItem() {}

    // Serialize to pipe-separated line: id|name|price
    string serialize() const {
        ostringstream oss;
        oss << fixed << setprecision(2)
            << id << "|" << name << "|" << price;
        return oss.str();
    }

    // Deserialize from pipe-separated line; returns true on success
    static bool deserialize(const string &line, MenuItem &out) {
        istringstream iss(line);
        string token;

        if (!getline(iss, token, '|')) return false;
        try { out.id = stoi(token); } catch (...) { return false; }

        if (!getline(iss, token, '|')) return false;
        out.name = token;

        if (!getline(iss, token, '|')) return false;
        try { out.price = stod(token); } catch (...) { return false; }

        return true;
    }

    // Print a single table row
    void printRow(ostream &os) const {
        os << "| "
           << left  << setw(6)  << id
           << " | "
           << left  << setw(14) << name
           << " | "
           << right << setw(5)  << 2 // Assuming the units are 2 for this example.
           << " | "
           << right << setw(8)  << fixed << setprecision(2) << price * 2 // units * price for simplicity
           << " |\n";
    }

    // Print the table header
    static void printHeader(ostream &os) {
        os << "--------------------------------------------------\n";
        os << "| " 
           << left  << setw(6)  << "SaleID"
           << " | "
           << left  << setw(14) << "Item"
           << " | "
           << right << setw(5)  << "Units"
           << " | "
           << right << setw(8)  << "Total"
           << " |\n";
        os << "--------------------------------------------------\n";
    }
};

// ─────────────────────────────────────────────
// Sale helper struct (for loading sales.txt)
// ─────────────────────────────────────────────
struct Sale {
    int    saleId;
    int    menuId;
    int    unitsSold;
    double totalPrice;
};

// ─────────────────────────────────────────────
// recordSale: appends one sale line to salesFile
// Format: saleId|menuId|unitsSold|totalPrice
// ─────────────────────────────────────────────
bool recordSale(const string &salesFile, int saleId,
                const MenuItem &mn, int units)
{
    ofstream ofs(salesFile, ios::app);
    if (!ofs.is_open() || ofs.fail()) {
        cerr << "Error: Cannot open " << salesFile << " for writing.\n";
        return false;
    }

    double total = mn.price * units;
    ofs << fixed << setprecision(2)
        << saleId << "|" << mn.id << "|" << units << "|" << total << "\n";

    if (ofs.fail()) {
        cerr << "Error: Write to " << salesFile << " failed.\n";
        return false;
    }
    return true;
}

// ─────────────────────────────────────────────
// loadMenu: loads menu items from menuFile
// Returns number of items loaded
// ─────────────────────────────────────────────
int loadMenu(const string &menuFile, MenuItem arr[], int maxCount)
{
    ifstream ifs(menuFile);
    if (!ifs.is_open() || ifs.fail()) {
        cerr << "Error: Cannot open " << menuFile << " for reading.\n";
        return 0;
    }

    int count = 0;
    string line;
    while (count < maxCount && getline(ifs, line)) {
        if (line.empty()) continue;
        MenuItem item;
        if (MenuItem::deserialize(line, item)) {
            arr[count++] = item;
        } else {
            cerr << "Warning: Skipping malformed line: " << line << "\n";
        }
    }
    return count;
}

// ─────────────────────────────────────────────
// Helper: load sales from salesFile
// Returns number of sales loaded
// ─────────────────────────────────────────────
int loadSales(const string &salesFile, Sale arr[], int maxCount)
{
    ifstream ifs(salesFile);
    if (!ifs.is_open() || ifs.fail()) {
        cerr << "Error: Cannot open " << salesFile << " for reading.\n";
        return 0;
    }

    int count = 0;
    string line;
    while (count < maxCount && getline(ifs, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string tok;
        Sale s;
        try {
            if (!getline(iss, tok, '|')) continue; s.saleId     = stoi(tok);
            if (!getline(iss, tok, '|')) continue; s.menuId     = stoi(tok);
            if (!getline(iss, tok, '|')) continue; s.unitsSold  = stoi(tok);
            if (!getline(iss, tok, '|')) continue; s.totalPrice = stod(tok);
            arr[count++] = s;
        } catch (...) {
            cerr << "Warning: Skipping malformed sale line: " << line << "\n";
        }
    }
    return count;
}

// ─────────────────────────────────────────────
// Helper: find a MenuItem by id in array
// ─────────────────────────────────────────────
const MenuItem* findMenuItem(const MenuItem arr[], int count, int id)
{
    for (int i = 0; i < count; i++)
        if (arr[i].id == id) return &arr[i];
    return nullptr;
}

// ─────────────────────────────────────────────
// main
// ─────────────────────────────────────────────
int main()
{
    const string menuFile  = "menu.txt";
    const string salesFile = "sales.txt";
    const int    MAX       = 100;

    // --- 1. Write two menu items to menu.txt ---
    {
        ofstream ofs(menuFile);   // overwrite / create fresh
        if (!ofs.is_open()) {
            cerr << "Error: Cannot create " << menuFile << "\n";
            return 1;
        }
        MenuItem m1(1, "Paratha Daal", 60.00);
        MenuItem m2(2, "Dim Bun",      17.00);
        ofs << m1.serialize() << "\n";
        ofs << m2.serialize() << "\n";
    }

    // --- 2. Load menu from file ---
    MenuItem menu[MAX];
    int menuCount = loadMenu(menuFile, menu, MAX);
    if (menuCount == 0) {
        cerr << "No menu items loaded.\n";
        return 1;
    }

    // --- 3. Record two sales ---
    // Clear sales file first so we start fresh each run
    { ofstream ofs(salesFile); }   // truncate

    const MenuItem *item1 = findMenuItem(menu, menuCount, 1); // Paratha Daal
    const MenuItem *item2 = findMenuItem(menu, menuCount, 2); // Dim Bun

    if (item1) recordSale(salesFile, 1, *item1, 2);  // 2 × 60.00 = 120.00
    if (item2) recordSale(salesFile, 2, *item2, 3);  // 3 × 17.00 =  51.00

    // --- 4. Load sales and print Daily Sales Summary ---
    Sale sales[MAX];
    int saleCount = loadSales(salesFile, sales, MAX);

    // Column widths to match sample:
    // | SaleID | Item           | Units | Total  |
    // Separator line is 50 chars
    cout << "--------------------------------------------------\n";
    cout << "| "
         << left  << setw(6)  << "SaleID"
         << " | "
         << left  << setw(14) << "Item"
         << " | "
         << left  << setw(5)  << "Units"
         << " | "
         << left  << setw(8)  << "Total"
         << "|\n";
    cout << "--------------------------------------------------\n";

    for (int i = 0; i < saleCount; i++) {
        const MenuItem *mi = findMenuItem(menu, menuCount, sales[i].menuId);
        string itemName = mi ? mi->name : "Unknown";

        // Format total as string for left-aligned output
        ostringstream totalStr;
        totalStr << fixed << setprecision(2) << sales[i].totalPrice;

        cout << "| "
             << left  << setw(6)  << sales[i].saleId
             << " | "
             << left  << setw(14) << itemName
             << " | "
             << left  << setw(5)  << sales[i].unitsSold
             << " | "
             << left  << setw(8)  << totalStr.str()
             << "|\n";
    }
    cout << "--------------------------------------------------\n";
    cout << "Loaded data for " << saleCount << " sales from " << salesFile << "\n";

    return 0;
} 