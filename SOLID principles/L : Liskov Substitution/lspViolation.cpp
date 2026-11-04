/**
 * Liskov Substitution Principle (LSP) - VIOLATION EXAMPLES
 * 
 * "Objects of a superclass shall be replaceable with objects of its subclasses
 *  without breaking the application."
 * 
 * In this file, we see TWO classic ways developers accidentally violate LSP:
 * 
 * VIOLATION 1: Breaking State Invariants (The Rectangle & Square Problem)
 * - `Square` inherits from `Rectangle`.
 * - `Rectangle`'s contract allows `length` and `breadth` to change independently.
 * - `Square` overrides `setLength()` and `setBreadth()` to force both sides to be equal.
 * - When a client passes a `Square` into a function expecting a `Rectangle`,
 *   the area calculation silently produces the WRONG result!
 * 
 * VIOLATION 2: Unimplemented / Throwing Methods (The BankAccount & FixedDeposit Problem)
 * - `BankAccount` defines `deposit()` and `withdraw()`.
 * - `FixedDepositAccount` inherits from `BankAccount`, but throws an exception inside `withdraw()`.
 * - When client code substitutes a `FixedDepositAccount` where a `BankAccount` is expected,
 *   the program crashes at runtime!
 */

#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

// =============================================================================
// VIOLATION 1: Rectangle vs. Square (Breaking Class Invariant)
// =============================================================================
class Rectangle {
protected:
    double length;
    double breadth;

public:
    Rectangle(double l, double b) : length(l), breadth(b) {}
    virtual ~Rectangle() = default;

    virtual void setLength(double l) { length = l; }
    virtual void setBreadth(double b) { breadth = b; }

    double area() const { return length * breadth; }
};

class Square : public Rectangle {
public:
    explicit Square(double side) : Rectangle(side, side) {}

    // VIOLATION: To maintain "Square-ness", changing length also secretly changes breadth!
    void setLength(double l) override {
        length = l;
        breadth = l;
    }

    void setBreadth(double b) override {
        length = b;
        breadth = b;
    }
};

// Client function written against the base class `Rectangle`
void resizeAndVerifyRectangle(Rectangle& rect) {
    rect.setLength(10);
    rect.setBreadth(5);

    // Client rightfully expects: 10 * 5 = 50
    cout << "Expected Area: 50 | Actual Area: " << rect.area() << "\n";
    if (rect.area() != 50) {
        cout << "[LSP BROKEN!] Subtype altered the expected behavior of Rectangle!\n\n";
    } else {
        cout << "[OK] Behavior matches Rectangle contract.\n\n";
    }
}

// =============================================================================
// VIOLATION 2: BankAccount vs. FixedDepositAccount (Breaking Behavioral Contract)
// =============================================================================
class BankAccount {
protected:
    double balance;

public:
    explicit BankAccount(double initialBalance) : balance(initialBalance) {}
    virtual ~BankAccount() = default;

    virtual void deposit(double amount) {
        balance += amount;
        cout << "Deposited $" << amount << " | Balance: $" << balance << "\n";
    }

    // Base class contract promises that ANY BankAccount can withdraw money:
    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrew $" << amount << " | Balance: $" << balance << "\n";
        } else {
            cout << "Insufficient funds!\n";
        }
    }
};

class SavingsAccount : public BankAccount {
public:
    explicit SavingsAccount(double b) : BankAccount(b) {}
};

class FixedDepositAccount : public BankAccount {
public:
    explicit FixedDepositAccount(double b) : BankAccount(b) {}

    // VIOLATION: Fixed Deposit cannot be withdrawn arbitrarily, so it throws!
    void withdraw(double amount) override {
        throw logic_error("LSP Violation: Withdrawals are NOT allowed on Fixed Deposit accounts!");
    }
};

// Client function that expects ANY `BankAccount` to honor `withdraw()`
void processMonthlyAutoDebit(BankAccount& account, double billAmount) {
    try {
        account.withdraw(billAmount);
    } catch (const exception& e) {
        cout << "[CRASH!] " << e.what() << "\n";
    }
}

int main() {
    cout << "=== 1. Testing Rectangle vs Square ===\n";
    Rectangle rect(2, 3);
    resizeAndVerifyRectangle(rect); // Works: 10 * 5 = 50

    Square sq(4);
    resizeAndVerifyRectangle(sq);   // FAILS! Expected 50, got 25!

    cout << "=== 2. Testing BankAccount vs FixedDepositAccount ===\n";
    SavingsAccount savings(500);
    FixedDepositAccount fd(1000);

    processMonthlyAutoDebit(savings, 100); // Works fine
    processMonthlyAutoDebit(fd, 100);      // Crashes with exception!

    return 0;
}

