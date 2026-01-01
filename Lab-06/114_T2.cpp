#include<iostream>
#include<string>
using namespace std;

class PlayerStats{
    private:
        string username;
        long long xp;
        int gold;
    public:
        // Constructors
        PlayerStats(const string &name = "Unknown", long long exp = 0, int gld = 0)
            : username(name), xp(0), gold(0) 
        {
            setXp(exp);
            setGold(gld);
        }

        // Getters
        string getUsername() const {
            return username;
        }

        long long getXp() const {
            return xp;
        }

        int getGold() const {
            return gold;
        }

        // Setters
        void setUsername(string name) {
            username = name;
        }

        void setXp(int exp) {
            if(exp < 0)
                cout << "Invalid" << endl;
            else
                xp = exp;
        }
        
        void setGold(int gld) {
            gold = gld;
        }

        PlayerStats& operator+=(const PlayerStats& other) {
            xp += other.xp;
            gold += other.gold;
            return *this;
        }

        // Operator += (add gold only)
        PlayerStats& operator+=(int goldAmount) {
            if (goldAmount < 0) {
                throw std::invalid_argument("Cannot add negative gold.");
            }
            gold += goldAmount;
            return *this;
        }

        // Operator ==
        bool operator==(const PlayerStats& other) const {
            return xp == other.xp && gold == other.gold;
        }

        // Operator !=
        bool operator!=(const PlayerStats& other) const {
            return !(*this == other);
        }

        // Operator >= (compare XP only)
        bool operator>=(const PlayerStats& other) const {
            return xp >= other.xp;
        }

        
};
