#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>

using namespace std;

class FileOpenException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Cannot open the file.";
    }
};

class MalformedSaleRecordException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Malformed sale record.";
    }
};

class UnknownMenuItemException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Unknown menu item referenced in sale.";
    }
};

class NegativeUnitsException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Sale units cannot be zero or negative.";
    }
};

class MenuItem {
public:
    int id;
    string name;
    double price;

    MenuItem(int id = 0, const string &name = "", double price = 0.0)
        : id(id), name(name), price(price) {}

    ~MenuItem() {}

    string serialize() const {
        ostringstream oss;
        oss << fixed << setprecision(2)
            << id << "|" << name << "|" << price;
        return oss.str();
    }

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

    void printRow(ostream &os) const {
        os << "| "
           << left  << setw(6)  << id
           << " | "
           << left  << setw(14) << name
           << " | "
           << right << setw(5)  << fixed << setprecision(2) << price
           << " |\n";
    }

    static void printHeader(ostream &os) {
        os << "--------------------------------------------------\n";
        os << "| "
           << left  << setw(6)  << "ID"
           << " | "
           << left  << setw(14) << "Name"
           << " | "
           << right << setw(5)  << "Price"
           << " |\n";
        os << "--------------------------------------------------\n";
    }
};

class SaleRecord {
public:
    int saleId;
    int menuId;
    int units;
    double total;

    SaleRecord(int saleId = 0, int menuId = 0, int units = 0, double total = 0.0)
        : saleId(saleId), menuId(menuId), units(units), total(total) {}

    string serialize() const {
        ostringstream oss;
        oss << fixed << setprecision(2)
            << saleId << "|" << menuId << "|" << units << "|" << total;
        return oss.str();
    }

    static bool deserialize(const string &line, SaleRecord &out) {
        istringstream iss(line);
        string token;

        if (!getline(iss, token, '|')) return false;
        try { out.saleId = stoi(token); } catch (...) { return false; }

        if (!getline(iss, token, '|')) return false;
        try { out.menuId = stoi(token); } catch (...) { return false; }

        if (!getline(iss, token, '|')) return false;
        try { out.units = stoi(token); } catch (...) { return false; }

        if (!getline(iss, token, '|')) return false;
        try { out.total = stod(token); } catch (...) { return false; }

        return true;
    }

    void printRow(ostream &os) const {
        os << "| "
           << left  << setw(6)  << saleId
           << " | "
           << left  << setw(14) << "Sale"
           << " | "
           << right << setw(5)  << units
           << " | "
           << right << setw(8)  << fixed << setprecision(2) << total
           << " |\n";
    }

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

template <typename T, int MAX>
class FileRepository {
public:
    T items[MAX];
    int currSize;
    string fileName;

    FileRepository(const string &fileName)
        : currSize(0), fileName(fileName) {}

    bool add(const T &item) {
        if (currSize >= MAX) return false;

        items[currSize] = item;
        currSize++;

        ofstream ofs(fileName, ios::app);
        if (!ofs) return false;
        ofs << item.serialize() << endl;
        return true;
    }

    int load() {
        ifstream ifs(fileName);
        if (!ifs) return 0;

        int count = 0;
        string line;
        while (count < MAX && getline(ifs, line)) {
            if (line.empty()) continue;
            if (T::deserialize(line, items[count])) {
                count++;
            }
        }
        currSize = count;
        return count;
    }

    void printAll(ostream &os) const {
        T::printHeader(os);
        for (int i = 0; i < currSize; i++) {
            items[i].printRow(os);
        }
    }

    int count() const {
        return currSize;
    }
};

int main() {
    try {
        FileRepository<MenuItem, 100> menuRepo("menu.txt");
        FileRepository<SaleRecord, 500> salesRepo("sales.txt");

        MenuItem m1(1, "Paratha Daal", 60.00);
        MenuItem m2(2, "Dim Bun", 17.00);
        menuRepo.add(m1);
        menuRepo.add(m2);

        SaleRecord s1(1, 1, 2, 120.00);
        SaleRecord s2(2, 2, 3, 51.00);
        salesRepo.add(s1);
        salesRepo.add(s2);

        menuRepo.load();
        cout << "Menu Items:" << endl;
        menuRepo.printAll(cout);

        salesRepo.load();
        cout << "Sales Records:" << endl;
        salesRepo.printAll(cout);

    } catch (const FileOpenException &e) {
        cerr << e.what() << endl;
    } catch (const MalformedSaleRecordException &e) {
        cerr << e.what() << endl;
    } catch (const UnknownMenuItemException &e) {
        cerr << e.what() << endl;
    } catch (const NegativeUnitsException &e) {
        cerr << e.what() << endl;
    } catch (const std::bad_alloc &e) {
        cerr << "Error: Memory allocation failure." << endl;
    } catch (const std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}