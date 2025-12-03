#include <iostream>
using namespace std;

class PaladinHero {
private:
    int level;        
    int weaponDamage;   
    int manaPoints;     

    int calculateHolyDamage() const {
        return (level * 2) + weaponDamage + (0.1 * manaPoints); 
    }

public:
    PaladinHero(int lvl, int weapon, int mana) : level(lvl), weaponDamage(weapon), manaPoints(mana) {}

    ~PaladinHero() {
        cout << "The hero has fallen." << endl;
    }

    void attackMonster() {
        int holyDamage = calculateHolyDamage();
        cout << "Striking the enemy for " << holyDamage << " damage!" << endl;
        manaPoints--; 
    }

    void restAndRecover(int recoveryAmount) {
        manaPoints += recoveryAmount;
        level++;
        cout << "Mana restored by " << recoveryAmount << " units and level increased to " << level << endl;
    }
};

int main() {
    int l, w, m;
    cout << "level: ";
    cin >> l ;
    cout << "Weapon Damage: ";
    cin >> w;
    cout << "Mana Points: ";
    cin >> m;
    PaladinHero hero(l, w, m);
    hero.attackMonster();  
    int r;
    cout << "Recover Amount: ";
    cin >> r;
    hero.restAndRecover(r);

    return 0;
}
