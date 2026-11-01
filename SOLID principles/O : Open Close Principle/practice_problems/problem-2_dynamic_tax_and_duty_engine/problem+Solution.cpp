/*
The Scenario: 
- In an e-commerce platform, products attract different tax rules depending on their category and regulations:
i.e.
    - Grocery / Food: 0% tax (tax-exempt).
    - Electronics: 18% standard VAT.
    - Luxury Goods: 28% luxury tax + flat $10 green surcharge.

A junior developer implemented this using an ItemType enum and an if-else block inside a TaxCalculator class.

Next month, the business wants to support:
    - Prescription Medicines: 5% healthcare concession rate.
    - Imported Goods: 20% customs duty applied on top of the base price.


=> Your Task : Design and implement a clean, OCP-compliant tax calculation system in C++:

1. Define an abstract TaxRule (or TaxStrategy) interface.
2. Implement the concrete tax rules (GroceryTax, ElectronicsTax, LuxuryTax).
3. Implement MedicineTax as a pure extension — demonstrating that existing tax classes and the tax calculator remain 100% untouched.
4. Write a small driver in main() showing an order containing multiple items evaluated with their respective tax rules.

*/

#include <iostream>
using namespace std;

// ============================
// ABSTRACT CLASS
// ============================
class TaxStrategy { 
public:
    virtual double applyTax(double productPrice) const = 0;
    virtual ~TaxStrategy() = default;
};

class GroceryTax : public TaxStrategy {
    double rate;
public:
    GroceryTax(double taxRate) : rate(taxRate) {}

    double applyTax(double productPrice) const override{
        
        return productPrice + (productPrice * (rate/100));
    }
};

class ElectronicsTax : public TaxStrategy {
    double rate;
public:
    ElectronicsTax(double taxRate) : rate(taxRate) {};

    double applyTax(double productPrice) const override{
        return productPrice + (productPrice * (rate/100));
    }
};

class LuxuryTax : public TaxStrategy {
    double rate;
    double surCharges;
public:
    LuxuryTax(double taxRate, double surcharge) : rate(taxRate), surCharges(surcharge)  {};
    
    double applyTax(double productPrice) const override{
        return (productPrice + (productPrice * (rate/100)) +  surCharges);
    }
};

// EXTENSIONS : Additional Classes added later
class MedicinesTax : public TaxStrategy {
    double rate;
public:
    MedicinesTax(double taxRate) : rate(taxRate) {};

    double applyTax(double productPrice) const override{
        return productPrice + (productPrice * (rate/100));
    }
};

class ImportedGoodsTax : public TaxStrategy {
    double rate;
public:
    ImportedGoodsTax(double taxRate) : rate(taxRate) {};

    double applyTax(double productPrice) const override{
        return (productPrice + (productPrice * (rate/100)) );
    }
};

// TAX APPLIER - interacts with the abstract class
class TaxApplier{
public:
    double apply(const TaxStrategy& taxCategory, double productPrice) const {
        return taxCategory.applyTax(productPrice);
    }
};

int main(){

    struct taxRates{
        double grocery = 0;
        double electronic = 18;
        double luxuryGoods = 28;
        double medicine = 5;
        double importedGoods = 20;
    };
    
    TaxApplier tax;
    taxRates rate;

    GroceryTax groceries(rate.grocery);
    cout << "Bread Price (before tax) : $2" << endl; 
    cout << "Bread Price (after tax) : $" << tax.apply(groceries, 2) << endl;
    cout << "______________________________________________________" << endl; 


    ElectronicsTax electronics(rate.electronic);
    cout << "TV Price (before tax) : $100" << endl;
    cout << "TV Price (after tax) : $" << tax.apply(electronics, 100) << endl;
    cout << "______________________________________________________" << endl; 

    LuxuryTax luxary(rate.luxuryGoods, 10); // this point where we pass surcharges is a design choice
    // 1. Similar to LuxaryTax class, other classes can have a surcharge which is set to default 0, but when needed a surcharge as applicable can be passes and to the constructor.
    // 2. A value of surcharge can be passes directly in the constructor, or a utiliy like taxRates can be used and the surchange of various category can be sotred there as used as needed.
    cout << "Gold ring (before tax) : $2199" << endl;
    cout << "Gold ring (after tax) : $" <<  tax.apply(luxary, 2199) << endl;
    cout << "______________________________________________________" << endl; 

    MedicinesTax medicines(rate.medicine);
    cout << "Medicine ABC (before tax) = $15" << endl;
    cout << "Medicine ABC (after tax) = $" << tax.apply(medicines, 15) << endl;
    cout << "______________________________________________________" << endl; 

    ImportedGoodsTax importedGoods(rate.importedGoods);
    cout << "Wrist Watch (before tax) : $1000" << endl;
    cout << "Wrist Watch (after tax) : $" << tax.apply(importedGoods, 1000) << endl;
    cout << "______________________________________________________" << endl; 

    return 0;
}