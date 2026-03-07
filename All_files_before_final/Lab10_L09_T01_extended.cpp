#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
public:
    Person(string n = "", int a = 0) : name(n), age(a) {}

    void setName(string n) {name = n;}
    void setAge(int a) {age = a;}

    string getName() const {return name;}
    int getAge() const {return age;}
    virtual string getRole() const {return "General Person";}
    virtual void displayDetails() {
        cout << "Name: " << getName() << endl;
        cout << "Age: " << getAge() << endl;
    }
    
    virtual ~Person() {}
};
class Faculty : public Person {
private:
    string employeeID;
    string Department;
    int CoursesTaught;
public:
    Faculty(string n = "", int a = 0, string eID = "", string dept = "", int noCourse = 0)
     : Person(n, a), employeeID(eID), Department(dept), CoursesTaught(noCourse) {}
    
    void setEmpID(string eID) {employeeID = eID;}
    void setDept(string dept) {Department = dept;}
    void setNoCourse(int noCourse) {CoursesTaught = noCourse;}

    string getEmpID() const {return employeeID;}
    string getDept() const {return Department;}
    int getNoCourse() const {return CoursesTaught;}
    
    // Override getRole()
    string getRole() const override {return "Faculty Member";}

    // Override displayDetails()
    void displayDetails() override {
        Person::displayDetails();
        cout << "Employee ID: " << getEmpID() << endl;
        cout << "Department: " << getDept() << endl;
        cout << "Courses Taught: " << getNoCourse() << endl;
    }
    
    void assignNewCourse() {
        CoursesTaught++;
        cout << "Number of courses taught increased to " << CoursesTaught << endl;
    }
};
class Student : public Person {
private: 
    string studentID;
    float currentCGPA;
public:
    Student(string n = "", int a = 0, string ID = "", float cg = 0.0) 
           : Person(n, a), studentID(ID), currentCGPA(cg) {}

    void setStudentID(string ID) {studentID = ID;}
    void setCurrCGPA(float cg) {currentCGPA = cg;}

    string getID() const {return studentID;}
    float getCGPA() const {return currentCGPA;}

    string getRole() const override {return "Student";}
    
    void updateCGPA(float newCG) {
        currentCGPA = newCG;
        cout << "Updated CGPA: " << currentCGPA << endl;
    }
    void displayDetails() override {
        Person::displayDetails();
        cout << "ID: " << getID() << endl;
        cout << "CGPA: " << getCGPA() << endl;
    }
};
class ScholarshipStudent : public Student {
private:
    float scholarshipPercentage;
    float minCGPARequirement;
public:
    ScholarshipStudent(string n = "", int a = 0, string ID = "", float cg = 0.0, 
                       float sp = 0.0, float minCG = 0.0) 
                 : Student(n, a, ID, cg), scholarshipPercentage(sp), minCGPARequirement(minCG) {}
    
    void setScholarshipPercentage(float sp) {scholarshipPercentage = sp;}
    void setMinCGPARequirement(float minCG) {minCGPARequirement = minCG;}

    float getScholarshipPercentage() const {return scholarshipPercentage;}
    float getMinCGPARequirement() const {return minCGPARequirement;}

    string getRole() const override {return "Scholarship Student";}

    void displayDetails() override {
        Student::displayDetails();
        cout << "Scholarship Percentage: " << getScholarshipPercentage() << "%" << endl;
    }
    
    void checkEligibility() {
        if (getCGPA() >= minCGPARequirement) {
            cout << "Student maintains scholarship eligibility" << endl;
        } else {
            cout << "Student doesn't meet scholarship requirement" << endl;
        }
    }
    
    void updateCGPA(float newCG) {
        Student::updateCGPA(newCG);
    }
};

int main() {
    cout << "=== LAB TASK 1: Polymorphism with Person* Array ===" << endl;
    cout << endl;
    Person* arr[4];

    arr[0] = new Person("John Doe", 45);
    arr[1] = new Student("Alice Johnson", 20, "230041234", 3.8);
    arr[2] = new ScholarshipStudent("Bob Smith", 21, "230043156", 3.6, 75.0, 3.5);
    arr[3] = new Faculty("Dr. Sarah Williams", 50, "EMP2024", "Computer Science", 3);
    
    cout << "--- Calling displayDetails() on each person ---" << endl;
    cout << endl;

    for (int i = 0; i < 4; i++) {
        cout << "Person " << (i + 1) << ":" << endl;
        arr[i]->displayDetails();
        cout << endl;
    }
    
    cout << "--- Collecting Role Information ---" << endl;
    cout << endl;
    
    for (int i = 0; i < 4; i++) {
        cout << "Person " << (i + 1) << " Role: " << arr[i]->getRole() << endl;
    }
    
    cout << endl;
    cout << "--- Special Operations for Student Objects ---" << endl;
    cout << endl;
    
    // 'static' cast doesn't throw an error or return NULL 
    for (int i = 0; i < 4; i++) {
        Student* studentPtr = dynamic_cast<Student*>(arr[i]); // Casting from Base --> Derived
        if (studentPtr != nullptr) { // not Student or not Derived from Student, instead of performing the cast it returns NULL
            cout << "Found Student: " << studentPtr->getName() << endl;
            studentPtr->updateCGPA(studentPtr->getCGPA() + 0.1);
            cout << endl;
        }
    }
    
    cout << "--- Special Operations for ScholarshipStudent Objects ---" << endl;
    cout << endl;

    for (int i = 0; i < 4; i++) {
        ScholarshipStudent* scholarPtr = dynamic_cast<ScholarshipStudent*>(arr[i]);
        if (scholarPtr != nullptr) {
            cout << "Found Scholarship Student: " << scholarPtr->getName() << endl;
            scholarPtr->checkEligibility();
            cout << endl;
        }
    }
    
    cout << "--- Additional Test: Changing CGPA and Re-checking Eligibility ---" << endl;
    cout << endl;

    for (int i = 0; i < 4; i++) {
        ScholarshipStudent* scholarPtr = dynamic_cast<ScholarshipStudent*>(arr[i]);
        if (scholarPtr != nullptr) {
            cout << "Testing scholarship eligibility after CGPA drop:" << endl;
            scholarPtr->updateCGPA(3.0);
            scholarPtr->checkEligibility();
            cout << endl;
        }
    }

    for (int i = 0; i < 4; i++) {
        delete arr[i];
    }
    
    return 0;
}
