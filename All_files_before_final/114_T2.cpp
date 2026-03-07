#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

// ═══════════════════════════════════════════════════════════════════
//  MenuItem
//  File format: id|name|price
// ═══════════════════════════════════════════════════════════════════
class MenuItem {
public:
    int    id;
    string name;
    double price;

    MenuItem(int id = 0, const string &name = "", double price = 0.0)
        : id(id), name(name), price(price) {}
    ~MenuItem() {}

    // Serialize to pipe-separated line
    string serialize() const {
        ostringstream oss;
        oss << fixed << setprecision(2) << id << "|" << name << "|" << price;
        return oss.str();
    }

    // Parse a pipe-separated line; returns true on success
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

    static void printHeader(ostream &os) {
        os << "------------------------------------\n";
        os << "| " << left  << setw(4)  << "ID"
           << " | " << left  << setw(14) << "Name"
           << " | " << right << setw(7)  << "Price"
           << " |\n";
        os << "------------------------------------\n";
    }

    void printRow(ostream &os) const {
        os << "| " << left  << setw(4)  << id
           << " | " << left  << setw(14) << name
           << " | " << right << setw(7)  << fixed << setprecision(2) << price
           << " |\n";
    }
};

class SaleRecord {
public:
    int    saleId;
    int    menuId;
    int    units;
    double total;

    SaleRecord(int saleId = 0, int menuId = 0, int units = 0, double total = 0.0)
        : saleId(saleId), menuId(menuId), units(units), total(total) {}
    ~SaleRecord() {}

    string serialize() const {
        ostringstream oss;
        oss << fixed << setprecision(2)
            << saleId << "|" << menuId << "|" << units << "|" << total;
        return oss.str();
    }

    static bool deserialize(const string &line, SaleRecord &out) {
        istringstream iss(line);
        string token;
        try {
            if (!getline(iss, token, '|')) return false;
            out.saleId = stoi(token);

            if (!getline(iss, token, '|')) return false;
            out.menuId = stoi(token);

            if (!getline(iss, token, '|')) return false;
            out.units = stoi(token);

            if (!getline(iss, token, '|')) return false;
            out.total = stod(token);
        } catch (...) { return false; }
        return true;
    }

    // ── Table display ─────────────────────────────────────────────
    static void printHeader(ostream &os) {
        os << "--------------------------------------------------\n";
        os << "| " << left  << setw(6)  << "SaleID"
           << " | " << left  << setw(6)  << "MenuID"
           << " | " << right << setw(5)  << "Units"
           << " | " << right << setw(10) << "Total"
           << " |\n";
        os << "--------------------------------------------------\n";
    }

    void printRow(ostream &os) const {
        os << "| " << left  << setw(6)  << saleId
           << " | " << left  << setw(6)  << menuId
           << " | " << right << setw(5)  << units
           << " | " << right << setw(10) << fixed << setprecision(2) << total
           << " |\n";
    }
};

template <typename T, int MAX>
class FileRepository {
private:
    T items[MAX];
    int currSize;
    string fileName;
public:
    explicit FileRepository(const string &fileName)
        : currSize(0), fileName(fileName) {}

    int count() const { return currSize; }

    bool add(const T &item) {
        if (currSize >= MAX) {
            cerr << "Repository full (" << fileName << "). Cannot add item.\n";
            return false;
        }

        ofstream ofs(fileName, ios::app);
        if (!ofs.is_open() || ofs.fail()) {
            cerr << "Error: Cannot open '" << fileName << "' for writing.\n";
            return false;
        }

        ofs << item.serialize() << "\n";
        if (ofs.fail()) {
            cerr << "Error: Write to '" << fileName << "' failed.\n";
            return false;
        }

        items[currSize++] = item;
        return true;
    }

    int load() {
        ifstream ifs(fileName);
        if (!ifs.is_open() || ifs.fail()) {
            cerr << "Error: Cannot open '" << fileName << "' for reading.\n";
            return 0;
        }

        currSize = 0;
        string line;
        while (currSize < MAX && getline(ifs, line)) {
            if (line.empty()) continue;
            T item;
            if (T::deserialize(line, item)) {
                items[currSize++] = item;
            } else {
                cerr << "Warning: Skipping malformed line in '"
                     << fileName << "': \"" << line << "\"\n";
            }
        }
        return currSize;
    }

    void printAll(ostream &os) const {
        ostringstream headerBuf;
        T::printHeader(headerBuf);
        string headerStr = headerBuf.str();

        string separator = headerStr.substr(0, headerStr.find('\n') + 1);

        os << headerStr;
        for (int i = 0; i < currSize; i++)
            items[i].printRow(os);
        os << separator;
        os << "  " << currSize << " record(s) in '" << fileName << "'\n";
    }
};

int main()
{

    { ofstream ofs("menu.txt");  }  
    { ofstream ofs("sales.txt"); }   

    // ── 1. MenuItem repository ───────────────────────────────────
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║        Menu Item Repository          ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";

    FileRepository<MenuItem, 100> menuRepo("menu.txt");

    // Add items — each add() writes to disk AND stores in memory
    cout << "--- Adding items ---\n";
    menuRepo.add(MenuItem(1, "Paratha Daal",  60.00));
    menuRepo.add(MenuItem(2, "Dim Bun",       17.00));
    menuRepo.add(MenuItem(3, "Khichuri",      45.00));
    menuRepo.add(MenuItem(4, "Noodle Soup",   35.00));
    cout << menuRepo.count() << " item(s) added to menu.txt\n\n";

    // Load from disk into a fresh repo to demonstrate persistence
    cout << "--- Loading from disk ---\n";
    FileRepository<MenuItem, 100> menuRepo2("menu.txt");
    int loaded = menuRepo2.load();
    cout << loaded << " item(s) loaded from menu.txt\n\n";

    cout << "--- Menu Table ---\n";
    menuRepo2.printAll(cout);

    // ── 2. SaleRecord repository ─────────────────────────────────
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║        Sale Record Repository        ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";

    FileRepository<SaleRecord, 500> salesRepo("sales.txt");

    // Build sale records using menu prices
    cout << "--- Recording sales ---\n";
    salesRepo.add(SaleRecord(1, 1, 2, 1 * 60.00 * 2));   // 2 × Paratha Daal
    salesRepo.add(SaleRecord(2, 2, 3, 1 * 17.00 * 3));   // 3 × Dim Bun
    salesRepo.add(SaleRecord(3, 3, 1, 1 * 45.00 * 1));   // 1 × Khichuri
    salesRepo.add(SaleRecord(4, 4, 5, 1 * 35.00 * 5));   // 5 × Noodle Soup
    cout << salesRepo.count() << " sale(s) recorded to sales.txt\n\n";

    // Load from disk
    cout << "--- Loading from disk ---\n";
    FileRepository<SaleRecord, 500> salesRepo2("sales.txt");
    int salesLoaded = salesRepo2.load();
    cout << salesLoaded << " sale(s) loaded from sales.txt\n\n";

    cout << "--- Daily Sales Summary ---\n";
    salesRepo2.printAll(cout);

    return 0;
}