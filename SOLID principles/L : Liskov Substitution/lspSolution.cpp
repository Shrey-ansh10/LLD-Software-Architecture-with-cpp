// A derived class should be replacable with the base class. -> That's possible when Base class is Abstract and derived classes implement the abstraction. The base class work as interface.
// [Note: In addition to using abstract interfaces, every derived class must also honor the behavioral contract (preconditions, postconditions, and invariants) of that base class/interface.]
// E.g. : We all know that all Square are rectangle, but not all rectangle are square. 
// So if we think to build a rectangle class and then derive a square class based on it, that will violate likov substitution principle. Cos suppose at some place a client or user is expecting a behavior of rectangle with different length and breadth, and hence if he passes both those parameter and we use square class for it, that will create a trouble, cos square will take bothe lenght and breadth as same. Therefore we can say Square class cannot replace rectangle class. 

// Instead in such situation we need to do something like defining a shape class. Then from that shape class, two seperate dervived classes can be formed - Rectangle and Square.

#include <iostream>
using namespace std;

// =============================================================================
// SOLUTION 1: Shape Hierarchy (Fixing the Rectangle-Square Problem)
// Neither Rectangle nor Square inherits from the other. Both implement Shape.
// =============================================================================
class Shape{

    public:
    virtual double area() const = 0; // this is PURE VIRTUAL FUNCTION
    // a pure virtual function will always just declared with no body. The defination is left completely on the derived class to implement.
    // They provide Abstraction

    virtual ~Shape() = default;
};

class Rectangle : public Shape{
    double length;
    double breadth;

    public:
    Rectangle(double l, double b){
        length = l;
        breadth = b;
    }

    double area() const override{
              //   ↑      ↑
              //   │      └── inheritance/polymorphism safety
              //   │
              //   └── object-state / mutation guarantee
        return (length * breadth);
    }

};

class Square : public Shape{
    double sideLength;

    public:
    Square(double l){
        sideLength = l;
    }

    double area() const override{
        return (sideLength * sideLength);
    }
};

void printShapeArea(const Shape& shape) {
    // Any Shape subtype (Rectangle or Square) can be passed here safely!
    cout << "Computed Shape Area: " << shape.area() << "\n";
}

// =============================================================================
// SOLUTION 2: Banking Hierarchy (Fixing the FixedDeposit Withdrawal Problem)
// Only accounts that genuinely support withdrawals inherit from WithdrawableAccount.
// =============================================================================
class DepositableAccount {
public:
    virtual ~DepositableAccount() = default;
    virtual void deposit(double amount) = 0;
};

class WithdrawableAccount : public DepositableAccount {
public:
    virtual void withdraw(double amount) = 0;
};

class SavingsAccount : public WithdrawableAccount {
    double balance;
public:
    explicit SavingsAccount(double b) : balance(b) {}

    void deposit(double amount) override {
        balance += amount;
        cout << "[Savings] Deposited $" << amount << " | Balance: $" << balance << "\n";
    }

    void withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
            cout << "[Savings] Withdrew $" << amount << " | Balance: $" << balance << "\n";
        } else {
            cout << "[Savings] Insufficient funds!\n";
        }
    }
};

class FixedDepositAccount : public DepositableAccount {
    double balance;
public:
    explicit FixedDepositAccount(double b) : balance(b) {}

    void deposit(double amount) override {
        balance += amount;
        cout << "[FixedDeposit] Deposited $" << amount << " | Balance: $" << balance << "\n";
    }
    // Notice: No `withdraw()` method at all!
    // It is now impossible at compile-time to pass FixedDepositAccount to an auto-debit function.
};

void processMonthlyAutoDebit(WithdrawableAccount& account, double billAmount) {
    // 100% safe: Every subtype of WithdrawableAccount genuinely supports withdraw()!
    account.withdraw(billAmount);
}

int main(){
    cout << "=== 1. Shape Substitution (LSP Compliant) ===\n";
    Rectangle rect(10, 5);
    Square sq(4);

    printShapeArea(rect); // 50
    printShapeArea(sq);   // 16

    cout << "\n=== 2. Account Substitution (LSP Compliant) ===\n";
    SavingsAccount savings(500);
    FixedDepositAccount fd(1000);

    savings.deposit(200);
    fd.deposit(500);

    processMonthlyAutoDebit(savings, 100);
    // processMonthlyAutoDebit(fd, 100); // <-- Won't even compile! Caught at compile time instead of crashing at runtime!

    return 0;
}
