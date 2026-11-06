/**
 * LSP Formal Rules: Preconditions, Postconditions, and History Constraint
 * 
 * Alongside Signature Rules (like Return Type Covariance in `returnTypeRule.cpp`),
 * LSP requires subtypes to obey Method Rules and Property Rules:
 * 
 * 1. PRECONDITION RULE: A subtype CANNOT strengthen preconditions.
 *    (If Parent accepts a wide range of inputs, Child cannot reject valid inputs from that range.)
 * 
 * 2. POSTCONDITION RULE: A subtype CANNOT weaken postconditions.
 *    (If Parent guarantees a specific outcome after a method call, Child must also guarantee it.)
 * 
 * 3. HISTORY CONSTRAINT: A subtype CANNOT modify state that the Parent guarantees is immutable.
 */

#include <iostream>
#include <stdexcept>
using namespace std;

// =============================================================================
// 1. PRECONDITION RULE (Child cannot be STRICTER about inputs than Parent)
// =============================================================================
class DiscountApplier {
public:
    virtual ~DiscountApplier() = default;

    // Parent Precondition: `percent` is valid anywhere between 0 and 50.
    virtual double applyDiscount(double price, double percent) const {
        if (percent < 0 || percent > 50) {
            throw invalid_argument("Discount must be between 0% and 50%");
        }
        return price - (price * percent / 100.0);
    }
};

// BAD SUBTYPE (Violates Precondition Rule by STRENGTHENING the requirement):
class StrictDiscountApplier : public DiscountApplier {
public:
    // VIOLATION: Rejects anything above 20%!
    // A caller of `DiscountApplier` passing 30% (which is legal for Parent) will crash!
    double applyDiscount(double price, double percent) const override {
        if (percent < 0 || percent > 20) {
            throw invalid_argument("[LSP Violation] Strict child only allows up to 20%!");
        }
        return price - (price * percent / 100.0);
    }
};

// GOOD SUBTYPE (Follows Precondition Rule — accepts 0..50 or even wider 0..70):
class HolidayDiscountApplier : public DiscountApplier {
public:
    // VALID: Weakens precondition (accepts 0% to 70%, which covers all 0%..50% inputs!)
    double applyDiscount(double price, double percent) const override {
        if (percent < 0 || percent > 70) {
            throw invalid_argument("Holiday discount allows 0% to 70%");
        }
        return price - (price * percent / 100.0);
    }
};

// =============================================================================
// 2. POSTCONDITION RULE (Child cannot deliver LESS than Parent promised)
// =============================================================================
class Vehicle {
protected:
    int speed = 60;
public:
    virtual ~Vehicle() = default;
    int getSpeed() const { return speed; }

    // Parent Postcondition Guarantee: After calling brake(), speed MUST be lower than before.
    virtual void brake() {
        speed = max(0, speed - 20);
    }
};

// GOOD SUBTYPE: HybridCar slows down AND charges battery (honors postcondition: speed decreases)
class HybridCar : public Vehicle {
    int batteryCharge = 50;
public:
    void brake() override {
        speed = max(0, speed - 25); // Speed decreased -> Postcondition honored!
        batteryCharge += 5;         // Extra subtype behavior is fine
    }
};

// BAD SUBTYPE: BrokenCar increases speed when braking! (Violates postcondition)
class MalfunctioningCar : public Vehicle {
public:
    void brake() override {
        speed += 10; // VIOLATION: Caller expected speed to decrease after brake()!
    }
};

int main() {
    cout << "=== 1. Precondition Rule Test ===\n";
    HolidayDiscountApplier goodDiscount;
    StrictDiscountApplier badDiscount;

    // Client knows base class allows 30% discount:
    cout << "Holiday Discount (30% on $100): $" << goodDiscount.applyDiscount(100, 30) << "\n";
    try {
        cout << "Strict Discount (30% on $100): $" << badDiscount.applyDiscount(100, 30) << "\n";
    } catch (const exception& e) {
        cout << "Caught Error: " << e.what() << "\n";
    }

    cout << "\n=== 2. Postcondition Rule Test ===\n";
    HybridCar hybrid;
    cout << "Hybrid speed before brake: " << hybrid.getSpeed();
    hybrid.brake();
    cout << " | after brake: " << hybrid.getSpeed() << " (Postcondition honored)\n";

    MalfunctioningCar badCar;
    cout << "BadCar speed before brake: " << badCar.getSpeed();
    badCar.brake();
    cout << " | after brake: " << badCar.getSpeed() << " (Postcondition VIOLATED!)\n";

    return 0;
}

