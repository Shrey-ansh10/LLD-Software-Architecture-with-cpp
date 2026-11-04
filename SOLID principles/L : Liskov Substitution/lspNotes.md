# Liskov Substitution Principle (LSP)

> **"If $S$ is a subtype of $T$, then objects of type $T$ may be replaced with objects of type $S$ without altering any of the desirable properties of the program (correctness, task performed, etc.)."**  
> — Barbara Liskov (1987)

In simpler terms: **Any child class object must be seamlessly usable wherever its parent class object is expected, without breaking client code or surprising the caller.**

---

## 1. The Core Idea: Behavioral Subtyping vs. Real-World Taxonomy

A very common beginner mistake in OOP is assuming that **real-world "is-a"** automatically translates to **code inheritance (`public` inheritance)**:
* In geometry, a **Square** *is a* **Rectangle**.
* In biology, a **Penguin** *is a* **Bird** (and an **Ostrich** *is a* **Bird**).
* In banking, a **Fixed Deposit Account** *is a* **Bank Account**.

However, in software design, inheritance is **NOT** about taxonomy—it is about **Behavioral Subtyping ("behaves-as-a")**.
* When a class inherits from a Base class (concrete *or* abstract), it signs a **Behavioral Contract**.
* If a `Base` class promises 3 behaviors (`deposit()`, `withdraw()`, `getBalance()`), then **every** derived class must faithfully fulfill all 3 behaviors without throwing unexpected `"Unsupported Operation"` errors or breaking state assumptions.

> **Important Nuance on Abstract Base Classes**:  
> Making a base class abstract (an interface) is a great first step, **but an abstract base class alone does NOT guarantee LSP!** Even if `Base` is a pure virtual interface, if a derived class implements a virtual method by throwing `"Not Implemented"`, doing nothing silently, or breaking the expected postcondition, it **still violates LSP**.

---

## 2. Two Classic Scenarios

### Scenario A: The Rectangle vs. Square Problem (State Invariant Violation)
1. `Rectangle` has `setWidth(w)` and `setHeight(h)`. Its implicit contract (invariant) is: *width and height can be modified independently*.
2. `Square` inherits from `Rectangle`. To stay a square, overriding `setWidth(w)` sets **both** `width = w` and `height = w`.
3. **The Crash**: A client function takes `Rectangle& r`, calls `r.setWidth(10)` and `r.setHeight(5)`, and expects `r.area() == 50`. When a `Square` is passed in, `r.area()` returns `25`! `Square` broke `Rectangle`'s contract.
4. **The Fix**: Don't inherit `Square` from `Rectangle`. Both should independently implement a common `Shape` abstraction (`area()`).

### Scenario B: The Unimplemented / Throwing Method (Contract Violation)
1. `BankAccount` has `deposit(amount)` and `withdraw(amount)`.
2. `FixedDepositAccount` inherits from `BankAccount`, but fixed deposits don't allow withdrawals before maturity, so `withdraw()` throws `std::logic_error("Withdrawals not allowed!")`.
3. **The Crash**: A client loop iterating over `vector<BankAccount*>` to process auto-debits crashes as soon as it hits a `FixedDepositAccount`.
4. **The Fix**: Split the hierarchy by capability: `Account` (or `DepositableAccount` with `deposit()`) and `WithdrawableAccount` (adds `withdraw()`). `FixedDepositAccount` only inherits from `DepositableAccount`.

---

## 3. All Aspects of LSP: The 3 Formal Rule Categories

To truly master LSP, Barbara Liskov and Jeannette Wing defined three sets of rules that a subtype must obey:

```
                       Liskov Substitution Principle (LSP)
                                        │
        ┌───────────────────────────────┼───────────────────────────────┐
        ▼                               ▼                               ▼
1. Signature Rules              2. Property Rules               3. Method Rules
(Compiler-checked mostly)       (Object State Guarantees)       (Behavioral Contracts)
├── Argument Types              ├── Class Invariants            ├── Preconditions
├── Return Types (Covariance)   └── History Constraint          └── Postconditions
└── Exception Rule
```

### Category 1: Signature Rules (Syntax & Types)
1. **Method Argument Rule**:
   * The overridden method in the child must accept the same argument types (or wider/more general types, known as *contravariance*). In C++, overriding requires identical parameter types.
2. **Return Type Rule (Covariance)** *(See `lsp_rules/returnTypeRule.cpp`)*:
   * The return type of an overridden method in the child must be **identical or narrower (a subtype)** of the parent method's return type.
   * Example: If `Parent::getAnimal()` returns `Animal*`, then `Child::getAnimal()` can return `Dog*` (since every `Dog` is an `Animal`, the caller expecting an `Animal*` is never surprised!).
3. **Exception Rule**:
   * A child method should **not** throw new or broader exception types that the caller of the parent method is not prepared to handle.

### Category 2: Property Rules (Object State)
1. **Class Invariants**:
   * An **invariant** is a rule about an object's state that must *always* remain true (e.g., *"Speed of a car must never be negative"*, or *"In a Rectangle, changing width does not change height"*).
   * Subclasses must **preserve** all invariants of the parent class.
2. **History Constraint**:
   * A subclass should not allow state changes that the base class never permitted.
   * Example: If `ImmutablePoint` has no setters so its coordinates can never change after construction, deriving a `MutablePoint` that adds `setX()` violates the history constraint because clients holding `ImmutablePoint&` assume its coordinates never change over time.

### Category 3: Method Rules (Design by Contract)
1. **Preconditions Cannot Be Strengthened**:
   * A **precondition** is what must be true *before* a method is called (what the method demands from the caller).
   * If `Parent::setDiscount(double percent)` accepts any value from `0% to 50%`, the `Child::setDiscount(double percent)` **cannot** throw an error if `percent > 20%`.
   * *Why?* A caller following the parent's rule passes `30%`, and the child unexpectedly rejects it! (A child can accept *more* inputs, e.g., `0% to 70%`, which is *weakening* the precondition, never *fewer*).
2. **Postconditions Cannot Be Weakened**:
   * A **postcondition** is what the method guarantees to be true *after* it finishes.
   * If `Parent::brake()` guarantees that `speed` decreases, a `Child::brake()` **cannot** leave `speed` unchanged or increase it.

---

## 4. Relationship Between OCP and LSP

LSP and OCP are two sides of the same coin:
* **OCP** says: *"Use base abstractions and add new derived classes to extend behavior without modifying the caller."*
* **LSP** says: *"Those derived classes MUST honor the base abstraction's contract; otherwise, the caller will break or will have to be modified with `if-else` checks to handle the misbehaving child!"*

> **In short: Violating LSP automatically breaks OCP.**

---

## 5. Code Smells & Warning Signs (LLD Interview Tips)

How do you spot an LSP violation immediately in code?
1. **Dummy / Throwing Overrides**:
   ```cpp
   void fly() override {
       throw std::logic_error("Penguins can't fly!"); // 🚨 LSP VIOLATION!
   }
   ```
2. **Empty / No-op Overrides**:
   ```cpp
   void saveToDisk() override {
       // Do nothing because this is a ReadOnlyFile   // 🚨 LSP VIOLATION!
   }
   ```
3. **Type-Checking in Client Code (`dynamic_cast` / `typeid`)**:
   ```cpp
   void processAccount(BankAccount* acc) {
       // If client has to check which child it is before calling a base method:
       if (dynamic_cast<FixedDepositAccount*>(acc) == nullptr) {
           acc->withdraw(100); // 🚨 LSP VIOLATION!
       }
   }
   ```
4. **Unexpected Side Effects**:
   * Setter or action on one property silently corrupts another property that the base class treats as independent (like `Square::setWidth` changing `height`).

