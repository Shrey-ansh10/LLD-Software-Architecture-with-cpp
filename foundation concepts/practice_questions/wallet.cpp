/*
    Q2 — Encapsulation

    Build: Wallet

    Invariant: balance is never negative. Outside code must not read or write balance directly.

    API:
    - Wallet(int opening) — if opening < 0, store 0
    - void add(int amount) — ignore amount <= 0
    - bool spend(int amount) — if amount is positive and <= balance, subtract and return true; else no change, return false
    - int getBalance() const
    
    -> main must: open with 100, add(50), spend(30), print balance (120). Uncommenting w.balance = -5; must not compile.

    Done when: invalid operations leave the object valid.
*/

#include <iostream>
#include <memory>
using namespace std;

class Wallet{
    int balance;

    public:
    Wallet(int openingBal){
        if(openingBal < 0){
            balance = 0;
            // cout << "account created with balance 0" << endl;
        }else{
            balance = openingBal;
            // cout << "account created with balance : " << openingBal << endl;
        }
    }

    void add(int amount){
        if(amount > 0){
            balance = balance + amount;
            // cout << "Amount added to account : " << amount << endl;
            // cout << "Current Balance : " << balance << endl;
        }
    }

    bool spend(int amount){
        if(amount > 0 && amount <= balance){
            balance = balance - amount;
            // cout << "Amount spent : " << amount << "\nRemaining balance : " << balance << endl;
            return true;
        }else{
            // cout << "Invalid amount !!!" << endl;
            return false;
        }
    }

    int getBalance() const{
        // int bal = balance;
        return balance;
    }
};

int main(){

    Wallet w(100);
    w.add(50);
    w.spend(30);
    cout << "Current balance : " << w.getBalance() << endl;

    // w.balance = -5;

    return 0;
}