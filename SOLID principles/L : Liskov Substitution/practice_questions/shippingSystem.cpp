/*

Problem 2: Find the Contract Flaws — E-Commerce Coupon & Shipping System (Medium)

The Scenario : Not all LSP violations throw "Not Implemented". Often, subclasses compile fine and implement every method, but silently break the base class's Preconditions, Postconditions, or Invariants.

Examine the base class and its 3 subclasses in given code below.

Your Task :
1. For each of the three subclasses (DroneShipping, PromotionalShipping, HeavyFreightShipping), identify:
    - Does it follow or violate LSP?
    - Which specific LSP rule (Precondition Rule / Postcondition Rule) is involved, and what concrete input value (e.g. weightKg = ?) will break a client expecting ShippingProvider's contract?

    Ans: For the first class DroneShipping - yes it does break the LSP principle as it tightens the condition of base class, where base class provides precondition of shipping weights btw 0 to <50, the Drone class tightens it to a limit of <5. So it breaks the precondition rule.
        
        - The second class PromotionalShipping - Yes it breaks LSP, because it violates post condition rule. If the shipping weight is <=7 then the price goes to negative if flat $15 discount is applied, which is illogical in real world and as per post condition as well.
        
        - Third one is file as it's broadining the pre-condition, not strenghtening or tightening.



2. Write a corrected version in C++ where the postcondition bug is fixed and explain how you would handle DroneShipping's weight limitation without surprising a caller of ShippingProvider.

    - Corrected version of postcondition violated by PromotionalShipping is corrected in code.

    - Solution to correct DroneShipping is as Follow:
        
        -  Approach 1: Split the Hierarchy by Capability
            - If 'ShippingProvider' strictly means "can ship any package up to 50 kg", then 'DroneShipping' is not a 'ShippingProvider'.

            - Create a base 'LightShippingProvider' (contract: 0 < weightKg <= 5) and a 'StandardShippingProvider' (contract: 0 < weightKg <= 50), or keep 'DroneShipping' standalone (line 44).
            - Result: A function expecting StandardShippingProvider& (for up to 50 kg packages) cannot be passed a 'DroneShipping' object at compile time.



        - Approach 2: Make Eligibility Part of the Base Contract (canShip()) — Most Common in Real E-Commerce

            - Think about how Amazon Checkout actually works when you buy a 20 kg microwave: every carrier has different weight limits, so the base ShippingProvider contract shouldn't promise "everyone ships 50 kg". Instead, the base class exposes a query method:
            
            1. Caller checks eligibility first:
                virtual bool canShip(double weightKg) const {
                return weightKg > 0 && weightKg <= 50;
                }
            2. Contract Precondition is now: `canShip(weightKg) == true`
                virtual double calculateFee(double weightKg) const = 0;

            The drone class can now inherit this and before calaulating fee check if it can ship the weight or not and then calculate fee. This makes the calculate fee independent of weight.

            The base contract no longer promises "pass any weight up to 50 kg blindly". It says: "Call canShip(w) first; if true, calculateFee(w) is guaranteed to succeed and return >= 0.0."


        - Approach 3: Fallback to reguale shipping if the weight is more that deliverable.
        Have applied this in code.

*/

#include <iostream>
#include <stdexcept>
using namespace std;

class ShippingProvider {
public:
    virtual ~ShippingProvider() = default;

    // CONTRACT:
    // - Precondition: Accepts any package weight > 0 and <= 50 kg.
    // - Postcondition: Returns a valid shipping fee >= $0.0 (never negative).
    virtual double calculateFee(double weightKg) const {
        if (weightKg <= 0 || weightKg > 50) {
            throw std::invalid_argument("Weight must be between 0 and 50 kg.");
        }
        return weightKg * 2.5;
    }
};

// Subclass A
class DroneShipping : public ShippingProvider {
public:
    double calculateFee(double weightKg) const override {
        // OLD CODE _ Violating LSP
        // if (weightKg <= 0 || weightKg > 5) { // Drone can only carry up to 5kg!
        //     throw std::invalid_argument("Drone only supports up to 5 kg!");
        // }
        // return weightKg * 4.0;

        // NEW Code - also known as Composite Straterg(more about it in design patterns)
        if(weightKg > 0 && weightKg <= 5){
            return weightKg * 4.0 ;
        }else{ // Fallback to standard shipping
            return ShippingProvider::calculateFee(weightKg);
        }
    }
};

// Subclass B
class PromotionalShipping : public ShippingProvider {
public:
    double calculateFee(double weightKg) const override {
        if (weightKg <= 0 || weightKg > 50) {
            throw std::invalid_argument("Weight must be between 0 and 50 kg.");
        }
        // Applies a flat $15 discount on the shipping fee:
        // return (weightKg * 2.0) - 15.0 - old condition
        double fee = (weightKg * 2.0) - 15.0;
        return fee <= 0 ? 0 : fee;  
    }
};

// Subclass C
class HeavyFreightShipping : public ShippingProvider {
public:
    double calculateFee(double weightKg) const override {
        if (weightKg <= 0 || weightKg > 100) { // Supports up to 100kg!
            throw std::invalid_argument("Heavy freight supports up to 100 kg.");
        }
        return weightKg * 3.0;
    }
};

void printShippingQuote(const string& name, const ShippingProvider& provider, double weightKg) {
    try {
        double fee = provider.calculateFee(weightKg);
        cout << name << " (" << weightKg << " kg): $" << fee << "\n";
    } catch (const exception& e) {
        cout << name << " (" << weightKg << " kg) ERROR: " << e.what() << "\n";
    }
}

int main() {
    ShippingProvider standard;
    DroneShipping drone;
    PromotionalShipping promo;
    HeavyFreightShipping freight;

    cout << "=== 1. Light Package (3.0 kg) ===\n";
    printShippingQuote("Standard", standard, 3.0);
    printShippingQuote("Drone", drone, 3.0);      // Uses drone rate: $12
    printShippingQuote("Promo", promo, 3.0);      // Clamped to $0 instead of -$9
    printShippingQuote("Freight", freight, 3.0);

    cout << "\n=== 2. Medium Package (20.0 kg) ===\n";
    printShippingQuote("Standard", standard, 20.0);
    printShippingQuote("Drone", drone, 20.0);     // Falls back to standard rate: $50
    printShippingQuote("Promo", promo, 20.0);     // $25
    printShippingQuote("Freight", freight, 20.0);

    cout << "\n=== 3. Heavy Package (75.0 kg) ===\n";
    printShippingQuote("Freight", freight, 75.0); // Broadened precondition (<= 100 kg) works: $225

    return 0;
}
