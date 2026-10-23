/*
    Q1 — Class, object, constructor
    Build: Student

    Data (private): name (string), roll (int), marks (int)

    Required:

    Default constructor: "unknown", 0, 0
    Parameterized constructor: set all three
    display() const prints all three

    main must:

    Create one student with the default constructor and display()
    Create one student "Asha", 12, 88 on the stack and display()
    Create one student "Ravi", 15, 91 with make_unique and display() using ->
    
    Done when: both . and -> work, ints are never garbage, no new without unique_ptr.
*/


#include <iostream>
#include <memory>
using namespace std;

class Student{
    string name;
    int roll;
    int marks;

    public:
    Student(){
        name = "unknown";
        roll = 0;
        marks = 0;
    }

    Student(string name, int roll, int marks){
        this->name = name;
        this->roll = roll;
        this->marks = marks;
    }

    void display() const{
        cout << "Name of Student : " << name << endl;
        cout << "Roll no. of Student : " << roll << endl;
        cout << "Marks of Student : " << marks << endl;
    }

    ~Student(){}
};

int main(){

    Student s1;
    s1.display();
    
    Student s2("Asha", 12, 88);
    s2.display();

    auto s3 = make_unique<Student>("Ravi", 15, 91);
    s3->display();

    return 0;
}