#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Class: a user-defined type.
// It holds data members (state) AND member functions (behavior).
class Customer {
    // These are private by default. See encapsulation.cpp for why that matters.
    string name;
    int accountNumber;
    int accountBal;

public:
    // Constructor: called automatically when an object is created.
    // Its job is to initialize the object's data members.

    // Default constructor: no arguments. Always initialize members yourself.
    // If you leave ints uninitialized, they contain garbage.
    Customer()
        : name("unknown"), accountNumber(0), accountBal(0) {}

    // Parameterized constructor: set members from the caller's values.
    Customer(string n, int accNo, int bal)
        : name(n), accountNumber(accNo), accountBal(bal) {}

    // Destructor: called automatically when the object's lifetime ends.
    // Write cleanup here only for resources YOU own (heap memory, file handles, etc.).
    // string is cleaned up automatically, so this can stay empty.
    ~Customer() {}

    // Member function: behavior of the object. This is what makes a class more than a struct of data.
    void display() const {
        cout << name << " | acc: " << accountNumber << " | bal: " << accountBal << endl;
    }
};

int main() {
    // Object: one instance of the class.

    // 1) Stack allocation (default choice).
    // Destroyed automatically when it goes out of scope.
    Customer customer1("Bob", 101, 1000);
    customer1.display();

    Customer unnamed; // uses default constructor
    unnamed.display();

    // 2) Dynamic allocation only when lifetime must outlive this scope,
    //    or when you need polymorphic ownership (see runtimePolymorphism.cpp).
    // Prefer unique_ptr so cleanup is automatic (RAII). Do not bare-new and forget delete.
    auto customer2 = make_unique<Customer>("Seb", 102, 2050);
    customer2->display(); // pointer: use ->     object: use .
    
    // This is the older way of creating objects dynamically -> Customer* customer2 = new Customer("Seb", 102, 2050);
    // In this we had to manually clear the memory, but that caused many memory leak issue if the developer made mistake, so the uniquepoint and RAII were introduced in modern c++ to solve this.

    return 0;
}

/*
How should you generally create objects?

    1. Prefer : Stack Allocation
        Customer customer1("Bob", 101, 1000);
    
    - The object is created as a local variable on stack is the default choice in modern C++.
    
    2. Dynamic Allocation :
    Use dynamic allocation only when the object must outlive the current scope, or when ownership / polymorphism requires it.
    
    If you do need dynamic allocation:
        auto customer2 = make_unique<Customer>("Seb", 102, 2050);    

    => So generally a very good practical rule is: Prefer normal local objects unless you specifically need dynamic lifetime or polymorphic ownership.

    -> Use "dynamic allocation" only when the object's lifetime really needs to outlive the current scope, or when ownership/polymorphism requires it.
        
Constructor is called at creation. Destructor is called at the end of lifetime.
The compiler calls both; you do not call them yourself.
*/
