#include <iostream>
#include <string>
using namespace std;

class Meme {
private:
    string memeName;
    int originYear;
    int humorScore;
    bool isStillViral;

public:
    Meme() {
        memeName = "";
        originYear = 2010;
        humorScore = 50;
        isStillViral = false;
    }
    Meme(string name, int year, int score, bool viral) {
        memeName = name;
        setOriginYear(year);
        setHumorScore(score);
        isStillViral = viral;
    }

    string getMemeName() const {
        return memeName;
    }

    int getOriginYear() const {
        return originYear;
    }

    int getHumorScore() const {
        return humorScore;
    }

    bool getIsStillViral() const {
        return isStillViral;
    }

    void setMemeName(string name) {
        memeName = name;
    }

    void setOriginYear(int year) {
        if (year >= 2000 && year <= 2025) {
            originYear = year;
        } else {
            originYear = 2010;
        }
    }

    void setHumorScore(int score) {
        if (score >= 1 && score <= 100) {
            humorScore = score;
        } else {
            humorScore = 50;
        }
    }

    void setIsStillViral(bool viral) {
        isStillViral = viral;
    }

    void display() const {
        cout << "Meme: " << memeName 
             << " | Year: " << originYear 
             << " | Humor Score: " << humorScore 
             << " | Still Viral: " << (isStillViral ? "Yes" : "No") << endl;
    }
};

void archiveMemes(Meme collection[], int size) {
    cout << "=== Welcome to Dank Vault Meme Museum ===" << endl << endl;

    for (int i = 0; i < 5; i++) {
        string name;
        int year, score;
        char viral;

        cout << "Enter details for Meme " << (i + 1) << ":" << endl;
        
        cout << "  Meme Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "  Origin Year (2000-2025): ";
        cin >> year;
        
        cout << "  Humor Score (1-100): ";
        cin >> score;
        
        cout << "  Still Viral? (y/n): ";
        cin >> viral;
        
        bool isViral = (viral == 'y' || viral == 'Y');
        
        collection[i] = Meme(name, year, score, isViral);
        cout << endl;
    }

    srand(time(0));
    for (int i = 5; i < size; i++) {
        int randomYear = 2005 + rand() % 11; 
        int randomScore = 1 + rand() % 100; 
        
        collection[i] = Meme("Lost Meme", randomYear, randomScore, false);
    }
}

int main() {
    const int SIZE = 40;
    Meme memeCollection[SIZE];
    archiveMemes(memeCollection, SIZE);
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        if (memeCollection[i].getOriginYear() < 2010 && 
            memeCollection[i].getHumorScore() > 70) {
            memeCollection[i].display();
            count++;
        }
    }
    
    if (count == 0) {
        cout << "No vintage highly rated memes found in the archive." << endl;
    } else {
        cout << "\nTotal vintage highly rated memes: " << count << endl;
    }

    return 0;
}