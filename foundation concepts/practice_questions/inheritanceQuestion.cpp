/*
    Q3 — Inheritance (is-a), not an abstract interface
    Build: Person and Student where Student is a Person.

    Person:
    - protected string name
    - constructor from name
    - void introduce() const — prints I am <name>

    Student:
    - extra int roll
    - constructor (name, roll) must call Person’s constructor
    - void study() const — prints <name> (roll <roll>) is studying

    main: create Student("Asha", 12), call introduce() and study(). s.name from main must not compile.

    Done when: you can explain constructor order (Person first, then Student) and why name is protected not private here.

*/
#include <iostream>
using namespace std;


class Person {

protected: // the data will be accessable by clild but not from outside the class.
    string name;

public:
    Person(string name) : name(name) {
        // this->name = name; 
    }

    void introduce() const{
        cout << "Hi! I am " << name << endl;
    }

};

class Student : public Person {
    int roll;

public:
    Student(string name, int roll) : Person(name), roll(roll) {
        // this->roll = roll;
    }

    void study() const{
        cout << "Name :" << this->name << "\n Roll No. : " << this->roll << endl; 
        // cout << "Name :" << name << "\n Roll No. : " << roll << endl; // which one to use??
    }
};


int main(){

    Student s("Asha", 12);
    s.introduce();
    s.study();
    // s.name;

    return 0;
}