/* 

Static Member variables :

- Member variables of a class can be made static by using the "static" keyword. 
i.e. static totalCustomerCount;
- Unlike normal member variables, static member variables are shared by all objects of the class.

- Static members exist even if no objects of the class have been instantiated! 
This makes sense: they are created at the start of the program and destroyed at the end of the program, so their lifetime is not bound to a class object like a normal member.

- Essentially, static members are GLOBAL VARIABLES that live inside the scope region of the class. There is very little difference between a static member of a class and a normal variable inside a namespace.
- Static members are global variables that live inside the scope region of the class.

- Access static members using the class name and the scope resolution operator (::). 
i.e. Customer::totalCustomerCount;

- static member variables are essentially -> global variables, therefore you must explicitly define (and optionally initialize) the static member outside of the class, in the global scope.
i.e. int Customer::totalCustomerCount = 0;

*/

#include <iostream>
#include <string>
using namespace std;

/*
Static member variables:
- One copy shared by ALL objects of the class (not one per object).
- Exist for the whole program, even if zero objects exist.
- They live in the class's scope (not quite a free global: they still have access control).
- Access with ClassName::member
*/

class Customer {
    string name;
    int accountNumber, accountBal;

    // Modern way: declare + define + initialize inside the class.
    static inline int totalCustomers = 0;

    // static const int kMax = 100; // const statics can also be initialized in-class

public:
    Customer(string a, int b, int c)
        : name(a), accountNumber(b), accountBal(c) {
        totalCustomers++;
    }

    ~Customer() {
        totalCustomers--;
    }

    // Static member function: call without an object.
    // It can use static members only — NOT name / accountNumber / accountBal.
    static int getCustomersCount() {
        return totalCustomers;
    }
};

// Older style (still needed if you do NOT use inline):
// int Customer::totalCustomers = 0;
// Do not put that definition in a header — multiple includes → multiple definition linker error.
// Access specifiers do not apply to that out-of-class definition; private statics are still defined this way.
// initializing and declaring - done in the global scope
// If no initializer is provided, static member variables are zero-initialized by default.

// IMP : Do not put the static member definition in a header file(.h/.hpp) (much like a global variable, if that header file gets included more than once, you’ll end up with multiple definitions, which will cause a linker error).
int main() {
    cout << "before any object: " << Customer::getCustomersCount() << endl; // 0

    Customer customer1("Bob", 101, 1000);
    cout << "after customer1: " << Customer::getCustomersCount() << endl; // 1

    {
        Customer customer2("Seb", 102, 2050);
        cout << "after customer2: " << Customer::getCustomersCount() << endl; // 2
    } // customer2 destroyed here → count goes back to 1

    cout << "after customer2 dies: " << Customer::getCustomersCount() << endl; // 1

    return 0;
}
