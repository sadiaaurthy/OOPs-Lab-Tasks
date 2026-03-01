#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
public:
    Person(string str = "", int n = 0) : name(str), age(n) {}

    void setName(string str) {name = str;}
    void setAge(int n) {age = n;}

    string getName() const {return name;}
    int getAge() const {return age;}

    void displayDetails() {
        cout << "Name : "<< getName() << endl;
        cout << "Age : " << getAge() << endl;
    }
};

class Student : protected Person { //protected -> Don't want to access outside the classes just want it to be inherited in other classes
private:
    string studentID;
    float currentCGPA;
public: 
    Student(string str = "", int n = 0, string sID = "", float currCG = 0.0) : Person(str, n), studentID(sID), currentCGPA(currCG) {}
    
    void setSID(string sID) {studentID = sID;}
    void setCurrCG(float currCG) {currentCGPA = currCG;}

    string getSID() const {return studentID;}
    float getCurrCG() const {return currentCGPA;}

    void displayDetails() { //will be overridden
        Person :: displayDetails(); // didn't use scope resolution
        cout << "StudentID : " << getSID() << endl;
        cout << "CurrentCGPA : " << getCurrCG() << endl;
    }
    void UpdateCGPA(float newCG) {
        setCurrCG(newCG);
        cout << "CGPA updated to : " << getCurrCG() << endl;
    }
};

class ScholarshipStudent : protected Student {
private:
    float scholarshipPercentage, minCGPARequirement;
public: 
    ScholarshipStudent(string str = "", int n = 0, string sID = "", float currCG = 0.0, float schPer = 0.0, float minCG = 0.0)
     : Student(str, n, sID, currCG),  scholarshipPercentage(schPer), minCGPARequirement(minCG) {}

    void setSchPer(float schPer) {scholarshipPercentage = schPer;}
    void setMinCG(float minCG) {minCGPARequirement = minCG;}

    float getSchPer() const {return scholarshipPercentage;}
    float getMinCG() const {return minCGPARequirement;}

    void displayDetails () {
        Student :: displayDetails();
        cout << "Scholarship Percentage : " << getSchPer() << endl;
        cout << "MinCGPA Requirement : " << getMinCG() << endl;
    }
    void Eligibility() {
        if (getCurrCG() >= minCGPARequirement) {
            cout << "Eligible for scholarship.\n" ;
        } else {
            cout << "Sorry, not eligible.\n";
        }
    }
    void UpdateCG(float newCG) {
        Student :: UpdateCGPA(newCG);
    }
};

int main() {
    ScholarshipStudent stu("Raiha", 10, "30007289", 3.5, 80.0, 2.1);

    stu.displayDetails();
    stu.Eligibility();

    stu.UpdateCG(2.0);
    stu.displayDetails();
    stu.Eligibility(); 
    return 0;
}
