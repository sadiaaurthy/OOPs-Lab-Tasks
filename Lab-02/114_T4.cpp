#include <iostream>
using namespace std;

class EventTime {
private:
    int hour, minute, second;
    void normalize(long h, long m, long s) {
        long totalSeconds = h * 3600 + m * 60 + s;

        hour   = (totalSeconds / 3600) % 24;
        minute = (totalSeconds % 3600) / 60;
        second = totalSeconds % 60;
    }
public:
    int getHour()   { return hour; }
    int getMinute() { return minute; }
    int getSecond() { return second; }

    void setTime(long h, long m, long s) {
        if (h < 0 || m < 0 || s < 0) return;  
        normalize(h, m, s);
    }

    void addDuration(long h, long m, long s) {
        if (h < 0 || m < 0 || s < 0) return;
        long totalSeconds = hour * 3600 + minute * 60 + second;
        totalSeconds += h * 3600 + m * 60 + s;

        hour   = (totalSeconds / 3600) % 24;
        minute = (totalSeconds % 3600) / 60;
        second = totalSeconds % 60;
    }

    void printTime() {
        if (hour < 10) cout << "0";
        cout << hour << ":";

        if (minute < 10) cout << "0";
        cout << minute << ":";

        if (second < 10) cout << "0";
        cout << second;
    }
};


int main() {
    EventTime t;
    t.setTime(25, 130, 3675);  
    t.printTime(); 
    cout << endl;

    t.addDuration(5, 200, 5000);
    t.printTime();
    cout << endl;

    return 0;
}