#include <iostream>
using namespace std;

// Encapsulation: keep data private, expose a small public API that protects invariants.
// Outside code should not poke balance directly, or it could set a negative balance, etc.

class BankAccount {
    int balance; // private: only this class's member functions can touch it

public:
    BankAccount(int openingBalance) {
        if (openingBalance < 0) {
            balance = 0;
        } else {
            balance = openingBalance;
        }
    }

    void deposit(int amount) {
        if (amount > 0) {
            balance += amount;
        }
    }

    bool withdraw(int amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    // Getter: read-only view. const because it does not change the object.
    int getBalance() const {
        return balance;
    }
};

int main() {
    BankAccount acc(1000);

    // acc.balance = -50;  // NOT ALLOWED — balance is private

    acc.deposit(200);
    acc.withdraw(50);
    cout << acc.getBalance() << endl; // 1150

    return 0;
}

/*
private   → only this class
protected → this class + derived classes (see inheritance.cpp)
public    → anyone

Encapsulation is not "make everything private for the sake of it".
It is: hide representation, expose operations that keep the object valid.
*/
