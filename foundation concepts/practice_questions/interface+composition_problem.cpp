/*
    Q. — Mix: interface + composition
    
    Build:
    - abstract Payment with virtual bool pay(int amount) = 0
    - CardPayment — pay succeeds if amount <= 10000, print paid by card: <amount>
    - UpiPayment — always succeeds, print paid by upi: <amount>
    - Order has a unique_ptr<Payment> (composition)
    - Order::checkout(int amount) calls payment->pay(amount)
    
    
    TEST :
    - main : one order with card (500 and 20000), one order with UPI (500).

    Done when: Order does not mention Card vs UPI except at construction. Checkout talks only to Payment.
*/

#include <iostream>
#include <memory>
using namespace std;


class Payment{

public:
    virtual bool pay(int amount) = 0;
    virtual ~Payment() = default;
};


class CardPayment : public Payment {
public:
    bool pay(int amount) override {
        if(amount <= 10000){
            cout << "card: " << amount << endl;
            return true;
        } 
        cout << "payment failed." << endl;
        return false;
    }
};

class UpiPayment : public Payment {
public:
    bool pay(int amount) override {
        cout << "upi: " << amount << endl;
        return true;
    }
};

class Order{
    unique_ptr<Payment> p;

public:
    Order(unique_ptr<Payment> paymentMode){
        p = std::move(paymentMode);
    }
    
    bool checkout(int amount){
        return p->pay(amount);
    }
};


int main(){

    Order o1(make_unique<CardPayment>());

    o1.checkout(500);
    o1.checkout(20000);

    Order o2(make_unique<UpiPayment>());
    
    o2.checkout(500);

    return 0;
}