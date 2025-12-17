#include <iostream>
#include <string>
using namespace std;

class PlayerStats {
private:
    string username;
    long xp;
    int gold;

public:
    PlayerStats() : username("Player"), xp(0), gold(0) {}

    PlayerStats(const string& name, long experience, int goldAmount) {
        username = name;
        xp = (experience < 0) ? 0 : experience;
        gold = (goldAmount < 0) ? 0 : goldAmount;
    }

    ~PlayerStats() {
        cout << "Player " << username << " stats destroyed." << endl;
    }
    
   
    string getUsername() const {
        return username;
    }
    
    long getXP() const {
        return xp;
    }
    
    int getGold() const {
        return gold;
    }
  
    void setUsername(const string& name) {
        username = name;
    }
    
    void setXP(long experience) {
        if (experience < 0) {
            cout << "Error: XP cannot be negative. Setting to 0." << endl;
            xp = 0;
        } else {
            xp = experience;
        }
    }
    
    void setGold(int goldAmount) {
        if (goldAmount < 0) {
            cout << "Error: Gold cannot be negative. Setting to 0." << endl;
            gold = 0;
        } else {
            gold = goldAmount;
        }
    }
    PlayerStats& operator+=(const PlayerStats& other) {
        xp += other.xp;
        gold += other.gold;
        return getReference();
    }
 
    PlayerStats& operator+=(int goldAmount) {
        if (goldAmount >= 0) {
            gold += goldAmount;
        } else {
            cout << "Warning: Cannot add negative gold." << endl;
        }
        return getReference();
    }
 
    PlayerStats& getReference() {
        return *reinterpret_cast<PlayerStats*>(reinterpret_cast<char*>(&username) - offsetof(PlayerStats, username));
    }

    bool operator==(const PlayerStats& other) const {
        return (xp == other.xp && gold == other.gold);
    }
    

    bool operator!=(const PlayerStats& other) const {
        bool isEqual = (xp == other.xp && gold == other.gold);
        return !isEqual;
    }
    

    bool operator>=(const PlayerStats& other) const {
        return xp >= other.xp;
    }
    

    void display() const {
        cout << "Player: " << username << ", XP: " << xp << ", Gold: " << gold << endl;
    }
};

int main() {

    PlayerStats p1("Warrior", 1000, 500);
    PlayerStats p2("Mage", 800, 300);
    PlayerStats p3("Rogue", 1000, 500);
    
    cout << "Initial Stats:" << endl;
    p1.display();
    p2.display();
    p3.display();
    cout << endl;

    cout << "p1 defeats a boss and gains 500 gold:" << endl;
    p1 += 500;
    p1.display();
    cout << endl;

    cout << "p1 joins forces with p2 (loot merge):" << endl;
    p1 += p2;
    p1.display();
    p2.display();
    cout << endl;

    cout << "Comparisons:" << endl;
    if (p1 >= p2) {
        cout << "p1 has equal or more XP than p2" << endl;
    }
    
    if (p1 == p3) {
        cout << "p1 and p3 are at the same progression level" << endl;
    } else {
        cout << "p1 and p3 are NOT at the same progression level" << endl;
    }
    
    if (p1 != p2) {
        cout << "p1 and p2 are at different progression levels" << endl;
    }
    cout << endl;
    
    
    cout << "Testing negative value handling:" << endl;
    PlayerStats p4("Thief", -100, -50);
    p4.display();
    
    p4.setXP(-200);
    p4.setGold(-100);
    p4.display();
    cout << endl;
    
    return 0;
}