#include <iostream>
#include <string>
using namespace std;

// Inheritance: is-a relationship. Dog is an Animal.
// The derived class gets the base class's usable members and can add its own.
//
// A base class CAN have data members. "Methods only" is an INTERFACE / abstract class
// (see runtimePolymorphism.cpp). That is one style of inheritance, not the definition.

class Animal {
protected:
    // protected: derived classes can use this; outsiders cannot.
    string name;

public:
    Animal(string n) : name(n) {
        cout << "Animal constructor\n";
    }

    void eat() const {
        cout << name << " is eating\n";
    }

    string getName() const {
        return name;
    }
};

class Dog : public Animal {
    string breed; // extra data that only Dog has

public:
    // Call the base constructor explicitly. Base is constructed FIRST, then derived.
    Dog(string n, string b) : Animal(n), breed(b) {
        cout << "Dog constructor\n";
    }

    void bark() const {
        // name is accessible here because it is protected in Animal
        cout << name << " (" << breed << ") says woof\n";
    }
};

int main() {
    Dog d("Bruno", "Labrador");

    d.eat();  // inherited from Animal
    d.bark(); // defined on Dog
    cout << d.getName() << endl;

    // d.name; // NOT ALLOWED from main — name is protected, not public

    return 0;
}

/*
public inheritance: "Dog is an Animal" (the usual kind you want).

What gets inherited?
- public members stay usable through Dog
- protected members usable inside Dog, not from main
- private members of Animal exist in the Dog object, but Dog's code cannot name them

When to put const after a member function?
Does this function need to modify the object?
    YES → don't put const
    NO  → usually put const (getters, eat, bark, area...)

Without const you cannot call it on a const object. That is const-correctness.

What is the use of "override keyword" and when to use it?
-> It tells the compiler "I intend this function to override a virtual function inherited from my base class. Please verify that I actually did that."

Keywords like const, virtual, override are used to avoide runtime errors. Cos runtime errors are even harder to find.
*/
