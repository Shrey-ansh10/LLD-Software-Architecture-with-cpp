# Open / Closed Principle (OCP)

> **"Software entities (classes, modules, functions, etc.) should be open for extension, but closed for modification."**  
> — Bertrand Meyer (1988) & Robert C. Martin (Uncle Bob)

---

## 1. The Core Idea

The Open/Closed Principle is the primary driving force behind building extensible and maintainable software architectures.

* **Open for Extension**: You should be able to add **new features or new behaviors** to a system easily as business requirements change.
* **Closed for Modification**: You should achieve this **without modifying existing, already tested, and debugged source code**.

### The Big Question:
> *"How can you change what a module does without changing its code?"*

**The Answer: Abstraction and Polymorphism.**  
Instead of writing monolithic code full of conditionals (`if-else` or `switch` statements) that branch on specific types, you program to an **Abstract Interface**. New features are added by creating **new derived classes** that implement that interface, while existing caller code remains untouched.

---

## 2. Real-World Scenario: Payment Processing System

Imagine an e-commerce checkout platform that accepts payments:
* **Initial Requirements**: Support `Credit Card` and `PayPal`.
* **Future Requirements**: Support `UPI` (Google Pay/PhonePe), `Apple Pay`, `Cryptocurrency`, etc.

---

## 3. What Happens Without OCP? (`ocpViolation.cpp`)

In a naive implementation, a developer often uses an `enum` and a conditional ladder (`switch` or `if-else`):

```cpp
enum class PaymentType {
    CREDIT_CARD,
    PAYPAL
    // Later: UPI, CRYPTO...
};

class PaymentProcessor {
public:
    void processPayment(PaymentType type, double amount) {
        if (type == PaymentType::CREDIT_CARD) {
            // Credit card charging logic...
        } else if (type == PaymentType::PAYPAL) {
            // PayPal API logic...
        }
        // To add UPI: WE MUST MODIFY THIS FUNCTION!
    }
};
```

```
+-------------------------------------------------------------+
|                      PaymentProcessor                       |
+-------------------------------------------------------------+
| processPayment(PaymentType, double)                         |
|   ├── if (CREDIT_CARD) -> charge card                       |
|   ├── else if (PAYPAL) -> call PayPal API                   |
|   └── else if (UPI)    -> [MUST EDIT EXISTING CODE TO ADD!] |
+-------------------------------------------------------------+
```

### Problems & Pain Points:
1. **Regression Risk**:
   - Every time a new payment method is introduced, you must edit `PaymentProcessor.cpp`. A typo or bug introduced while adding `UPI` can accidentally break `Credit Card` or `PayPal` processing!
2. **Violation of Single Responsibility (SRP connection)**:
   - `PaymentProcessor` has to know the low-level details, secrets, and endpoints of every payment vendor in existence.
3. **Re-compilation & Re-testing Cascade**:
   - In compiled languages like C++, modifying `PaymentProcessor.h`/`cpp` causes all downstream files including it to recompile. QA must re-test the entire payment flow instead of just the new feature.
4. **Merge Conflicts**:
   - Two developers adding different payment methods (e.g. Developer A adds `UPI`, Developer B adds `Crypto`) will both edit the same `switch-case` block, resulting in painful merge conflicts.

---

## 4. What Happens With OCP? (`ocpSolution.cpp`)

We invert the dependency by creating an **Abstract Interface** (`PaymentMethod`). The `PaymentProcessor` depends solely on this abstraction:

```
                      +-------------------------+
                      |   «interface»           |
                      |      PaymentMethod      |
                      +-------------------------+
                      | + pay(amount): bool     |
                      +-------------------------+
                                   ▲
                                   │ (implements)
         ┌─────────────────────────┼─────────────────────────┐
         │                         │                         │
+------------------+      +------------------+      +------------------+
| CreditCardPayment|      |  PayPalPayment   |      |    UpiPayment    |
+------------------+      +------------------+      +------------------+
| + pay(amount)    |      | + pay(amount)    |      | + pay(amount)    |
+------------------+      +------------------+      +------------------+
                                                      (Newly Added!
                                                       ZERO changes to
                                                       existing code!)
```

```cpp
// The processor depends ONLY on the abstraction:
class PaymentProcessor {
public:
    void process(const PaymentMethod& method, double amount) {
        // Closed for modification! This logic NEVER needs to change.
        method.pay(amount);
    }
};
```

### Key Benefits:
* **Zero Blast Radius**: Adding `UpiPayment` means creating a **new file/class**. Existing `CreditCardPayment` and `PaymentProcessor` are not touched.
* **Independent Testing**: You write unit tests specifically for `UpiPayment`. The stability of already-deployed payment modes is guaranteed.
* **Plugin Architecture**: New modules can even be loaded dynamically as plugins without modifying the core engine.

---

## 5. Relationship Between SRP and OCP

* **SRP** tells us: *"Gather together the things that change for the same reason. Separate things that change for different reasons."*
* **OCP** tells us: *"Structure those separated things so that when new requirements arise, you add new code instead of changing old code."*

> If a class violates SRP (doing too many things), it almost certainly violates OCP (every new feature forces modifications to that class).

---

## 6. Code Smells & Warning Signs (LLD Interview Red Flags)

Look out for these classic signs of OCP violation in design interviews:
1. **The `enum` + `switch` or `if-else` Ladder**:
   * Inspecting `type`, `mode`, `shape`, or `role` using long conditional blocks to decide what algorithm to execute.
2. **`dynamic_cast` / `typeid` Inspection**:
   * Downcasting a base pointer to check if it's a specific derived type:
     ```cpp
     if (auto c = dynamic_cast<Circle*>(shape)) { ... }
     else if (auto r = dynamic_cast<Rectangle*>(shape)) { ... }
     ```
3. **Frequent Edits to Core Classes**:
   * If git history shows a central class being modified in almost every sprint, it is not closed for modification.
4. **Cascading Header Changes**:
   * Having to `#include "UPI.h"` and `#include "Crypto.h"` inside the core checkout service header.

