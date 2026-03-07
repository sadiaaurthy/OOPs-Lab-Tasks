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

    virtual void displayDetails() const {
        cout << "Name of Person: " << getName() << endl;
        cout << "Age of Person: " << getAge() << endl;
    }
    virtual string getRole() const {return "General Person";}
};

class Faculty : public Person {
private:
    string employeeID;
    string Department;
    int CoursesTaught;
public:
    Faculty(string str = "", int n = 0, string eID = "", string dept = "", int noCourse = 0) 
     : Person(str, n), employeeID(eID), Department(dept), CoursesTaught(noCourse) {}

    void setEID(string eID) {employeeID = eID;}
    void setDept(string dept) {Department = dept;}
    void setCourses(int noCourse) {CoursesTaught = noCourse;}

    string getEID() const {return employeeID;}
    string getDept() const {return Department;}
    int getCourses() const {return CoursesTaught;}

    void displayDetails() const override {
        Person::displayDetails();
        cout << "EmployeeID of Faculty: " << getEID() << endl;
        cout << "Department Name of Faculty: " << getDept() << endl;
        cout << "No of Courses a Faculty taught: " << getCourses() << endl;
    }

    string getRole() const override {return "Faculty Member";}

    void assignNewCourse() {
        CoursesTaught ++;
    }
};

class Student : public Person { //protected -> Don't want to access outside the classes just want it to be inherited in other classes
private:
    string studentID;
    float currentCGPA;
public: 
    Student(string str = "", int n = 0, string sID = "", float currCG = 0.0) 
    : Person(str, n), studentID(sID), currentCGPA(currCG) {}
    
    void setSID(string sID) {studentID = sID;}
    void setCurrCG(float currCG) {currentCGPA = currCG;}

    string getSID() const {return studentID;}
    float getCurrCG() const {return currentCGPA;}

    void displayDetails() const override { //will be overridden
        Person :: displayDetails(); // didn't use scope resolution
        cout << "StudentID : " << getSID() << endl;
        cout << "CurrentCGPA : " << getCurrCG() << endl;
    }

    string getRole() const override {return "Student";}
    
    void UpdateCGPA(float newCG) {
        setCurrCG(newCG);
        cout << "CGPA updated to : " << getCurrCG() << endl;
    }
};

class ScholarshipStudent : public Student { // diff in 'private' & 'protected' mode can only be seen in multilevel inheritance
private:
    float scholarshipPercentage, minCGPARequirement;
public: 
    ScholarshipStudent(string str = "", int n = 0, string sID = "", float currCG = 0.0, float schPer = 0.0, float minCG = 0.0)
     : Student(str, n, sID, currCG),  scholarshipPercentage(schPer), minCGPARequirement(minCG) {}

    void setSchPer(float schPer) {scholarshipPercentage = schPer;}
    void setMinCG(float minCG) {minCGPARequirement = minCG;}

    float getSchPer() const {return scholarshipPercentage;}
    float getMinCG() const {return minCGPARequirement;}
    void displayDetails () const override {
        Student :: displayDetails();
        cout << "Scholarship Percentage : " << getSchPer() << endl;
        cout << "MinCGPA Requirement : " << getMinCG() << endl;
        //cout << "Calling getSID from derived cls in private mode" << getSID();
    }

    string getRole() const override {return "ScholarshipStudent";}

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
    Person* arr[4];
    
    arr[0] = new Person("Horace Slughorn", 85);
    arr[1] = new Student("Salvor Hardin", 21, "23004115", 3.87); // inside Student, if constructor from Person class is inherited in a mode other than 'public' then the constructor Person(str, n) can't be accessed outside class
    arr[2] = new ScholarshipStudent("Sirius Black", 40, "23007682", 3.1, 85.00, 3.5);
    arr[3] = new Faculty("Hermione Granger", 21, "238459556", "Physics", 5);

    for (int i = 0; i < 4; i++) {
        cout << '('  << i + 1 << ") " << "Person's Role: " << arr[i]->getRole();
        cout << endl;
        arr[i]->displayDetails();
        cout << endl;
    }

    for (int i = 0; i < 4; i++) {
        Student* stuPtr = dynamic_cast<Student*> (arr[i]);
        if (stuPtr) {
            cout << "Found a student: " << stuPtr->getName() << endl;
            stuPtr->UpdateCGPA(stuPtr->getCurrCG() + 1.0);
            cout << endl;
        } 
    }

    for (int i = 0; i < 4; i++) {
        ScholarshipStudent* schPtr = dynamic_cast<ScholarshipStudent*>(arr[i]);
        if (schPtr) {
            cout << "Found a ScholarshipStudent, Name: " << schPtr->getName();
            cout << endl;
            schPtr->Eligibility(); 
            cout << endl;
        }
    }
}
