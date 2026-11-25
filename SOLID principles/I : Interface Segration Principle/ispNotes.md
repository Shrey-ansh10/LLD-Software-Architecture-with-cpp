# Interface Segregation Principle (ISP)

> **"Clients should not be forced to depend upon interfaces that they do not use."**  
> — Robert C. Martin (Uncle Bob)

In simpler terms: **Many small, role-specific interfaces are better than one large, general-purpose ("fat") interface.**

---

## 0. Prerequisite: What is an "Interface" in C++? (And How It Differs from an Abstract Class)

Unlike Java or C#, **C++ does not have a built-in `interface` keyword**. Instead, in C++, an interface is a **strict design pattern/convention** written using a `class` (or `struct`) that acts purely as a **behavioral contract**.

### A. The Anatomy & Rules of a True Interface in C++

```cpp
class IPrinter {
public:
    // 1. Virtual Destructor (MANDATORY!)
    virtual ~IPrinter() = default;

    // 2. ONLY Pure Virtual Functions (= 0)
    virtual void print(const std::string& document) const = 0;

    // 3. NO member variables (no state)
    // 4. NO user-defined constructors needed (no data to initialize)
};
```

To write a proper interface in C++, follow these **4 Golden Rules**:

1. **Only Pure Virtual Functions (`virtual ... = 0;`)**:
   * An interface defines **WHAT** an object can do, never **HOW** it does it. Every method in the contract is declared `virtual` and assigned `= 0` (no body).
2. **No Member Variables (Zero State)**:
   * An interface never holds non-static data members (like `int count;` or `string name;`). Keeping it stateless is what prevents the **Diamond Problem** when a class implements multiple interfaces.
3. **Always Provide a `virtual` Destructor (`virtual ~InterfaceName() = default;`)**:
   * **Why?** Clients often hold objects via an interface pointer (e.g., `IPrinter*` or `std::unique_ptr<IPrinter>`). If that pointer is deleted and the destructor is **not** `virtual`, C++ will only call `~IPrinter()` and **skip the derived class's destructor** (`~EnterpriseLaserJet()`), causing memory leaks and undefined behavior!
4. **What About Constructors?**:
   * You **cannot instantiate** an interface directly (`IPrinter p;` will fail to compile).
   * Because an interface has no member variables to initialize, you do **not** need to write a constructor for it (the compiler automatically generates an implicit default constructor so derived classes can construct cleanly).
   * **C++ Rule**: Constructors can **never** be `virtual` (because the virtual table pointer `vptr` hasn't been created yet until the constructor runs!).

---

### B. Interface vs. Abstract Class in C++

In C++ compiler terminology, any class with at least one pure virtual function (`= 0`) is an **Abstract Class**.  
Therefore: **Every Interface in C++ is an Abstract Class, but NOT every Abstract Class is an Interface!**

| Feature | Pure Interface (e.g., `IPrinter`, `IScanner`) | Abstract Base Class (e.g., `FuelVehicle`, `Document`) |
| :--- | :--- | :--- |
| **Purpose** | Defines a pure capability contract (**"Can-Do"** / Role) | Shares common state & partial behavior (**"Is-A"** base) |
| **Methods** | **100% pure virtual (`= 0`)** — zero method implementations | Mix of pure virtual (`= 0`) AND normal methods with bodies |
| **Member Variables** | **None** (Completely stateless) | **Allowed** (e.g., `protected: double fuelLevel;`) |
| **Constructors** | No custom constructors (nothing to initialize) | Has constructors to initialize its own member variables |
| **Destructor** | `virtual ~IInterface() = default;` | `virtual ~AbstractBase() = default;` |
| **Multiple Inheritance** | **100% Safe** (A class can implement many interfaces) | **Risky** (Inheriting state from multiple bases causes the Diamond Problem) |

```cpp
// 1. PURE INTERFACE: Zero state, 100% pure virtual (= 0)
class IPrintable {
public:
    virtual ~IPrintable() = default;
    virtual void print() const = 0;
};

// 2. ABSTRACT CLASS: Has state, a constructor, a concrete method, AND a pure virtual method
class Document {
protected:
    std::string author; // <-- Has member variable (state)
public:
    Document(const std::string& a) : author(a) {} // <-- Has constructor to init state
    virtual ~Document() = default;

    std::string getAuthor() const { return author; } // <-- Concrete shared method
    virtual void render() const = 0;                 // <-- Pure virtual method
};
```

---

## 1. The Core Idea of ISP: Two Sides of a "Fat Interface"

When an interface grows too large (a **Fat / Polluted Interface**), it hurts **two** groups of code:

### A. The Implementer Side (Classes inheriting from the interface)
Suppose an interface `IMachine` defines `print()`, `scan()`, `fax()`, and `staple()`.
* An `EnterpriseAllInOneMachine` can happily implement all four.
* A simple `HomeInkjetPrinter` can **only print**. Because it inherits from `IMachine`, the compiler forces it to also write bodies for `scan()`, `fax()`, and `staple()`.
* The developer is forced to leave them empty or throw `"Not Supported"` — **which immediately causes an LSP Violation!**

### B. The Client / Caller Side (Functions calling the interface) — *The True Core of ISP*
Even if every machine in your office *could* print, scan, and fax, consider a client function:
```cpp
void printInvoice(const IMachine& machine, const std::string& doc);
```
* `printInvoice()` **only** calls `machine.print(doc)`.
* Why is passing `IMachine` bad?
  1. **Unnecessary Coupling**: `printInvoice` is now coupled to `scan()`, `fax()`, and `staple()`.
  2. **Recompilation Cascade (C++ Specific)**: If the fax team changes the signature of `fax()` inside `IMachine.h` (e.g., adding a `CountryCode` parameter), **every file that includes `IMachine.h` only for printing must be recompiled and redeployed!**
  3. **Harder Testing / Mocking**: To unit-test `printInvoice()`, a test mock of `IMachine` must implement dummy versions of `scan()`, `fax()`, and `staple()` even though the test never calls them.

---

## 2. How ISP Connects to SRP and LSP (Classic Interview Question)

Wait — doesn't ISP sound a lot like **SRP** and **LSP**? Here is the exact distinction:

| Principle | Focuses On | Core Question |
| :--- | :--- | :--- |
| **SRP** | **Class Implementation** | *"Does this class have more than one business reason to change?"* |
| **LSP** | **Inheritance Hierarchy** | *"Can a child class safely replace its parent without breaking the contract?"* |
| **ISP** | **Client-Facing Interfaces** | *"Is any client or implementer forced to see or implement methods it doesn't use?"* |

> **Can a class follow SRP while violating ISP? Or follow ISP with a multi-capability class?**  
> **Yes!** Imagine an `AllInOnePrinter` hardware driver class. It legitimately controls one physical device that prints, scans, and faxes. Instead of exposing one giant `IAllInOne` interface to everyone, it implements three segregated interfaces: `IPrinter`, `IScanner`, and `IFax`.  
> Now, a printing client only asks for `IPrinter&`, and a scanning client only asks for `IScanner&`!

---

## 3. What Happens Without ISP? (`ispViolation.cpp`)

```
            +---------------------------------------+
            |             «interface»               |
            |          IMultiFunctionDevice         |  <-- FAT INTERFACE
            +---------------------------------------+
            | + print(doc)                          |
            | + scan(doc)                           |
            | + fax(doc, number)                    |
            +---------------------------------------+
                       ▲                 ▲
                       │                 │
     ┌─────────────────┘                 └─────────────────┐
     │                                                     │
+--------------------------+                 +--------------------------+
|   EnterpriseOfficeJet    |                 |     BasicHomePrinter     |
+--------------------------+                 +--------------------------+
| + print(doc)   [OK]      |                 | + print(doc)   [OK]      |
| + scan(doc)    [OK]      |                 | + scan(doc)    [THROWS!] | <-- Forced to fake it!
| + fax(doc, #)  [OK]      |                 | + fax(doc, #)  [THROWS!] | <-- Violates LSP too!
+--------------------------+                 +--------------------------+
```

---

## 4. What Happens With ISP? (`ispSolution.cpp`)

We split the fat interface into **Role Interfaces** (each representing a single capability):

```
+--------------------+     +--------------------+     +--------------------+
|    «interface»     |     |    «interface»     |     |    «interface»     |
|      IPrinter      |     |      IScanner      |     |        IFax        |
+--------------------+     +--------------------+     +--------------------+
| + print(doc)       |     | + scan(doc)        |     | + fax(doc, number) |
+--------------------+     +--------------------+     +--------------------+
      ▲     ▲   ▲                ▲         ▲                    ▲
      │     │   │                │         │                    │
      │     │   └───────────┬────┘         └───────┬────────────┘
      │     └───────────────│──────────────────────│
+------------------+  +------------------+  +------------------------+
| BasicHomePrinter |  | OfficePhotocopier|  |  EnterpriseOfficeJet   |
| (Only IPrinter)  |  | (Print + Scan)   |  | (Print + Scan + Fax)   |
+------------------+  +------------------+  +------------------------+
```

### Wait — Doesn't `class EnterpriseOfficeJet : public IPrinter, public IScanner, public IFax` cause the C++ Diamond Problem?
* **No!** Recall from LSP Problem 4: the Diamond Problem happens when you inherit **state (member variables)** or **conflicting method implementations** from a shared base class.
* Pure interfaces (`IPrinter`, `IScanner`, `IFax`) have:
  * **Zero member variables** (no state)
  * **Zero method bodies** (only pure virtual `= 0` functions and a `virtual` destructor)
* Combining multiple pure interfaces in C++ is 100% safe, standard, and idiomatic!

---

## 5. Code Smells & Warning Signs (How to Spot ISP Violations)

1. **"Not Implemented" / No-op Implementations**:
   * A class implements an interface of 8 methods, leaves 5 of them empty, or throws `std::logic_error("Unsupported")`.
2. **Bloated Interfaces with Unrelated Verbs**:
   * An interface named `IWorker`, `IManager`, or `IService` with 10+ methods covering completely different use cases (e.g., `work()`, `eatLunch()`, `takeVacation()`, `receiveSalary()` — a `RobotWorker` works, but doesn't eat lunch or take vacations!).
3. **Clients Passing `nullptr` or Dummy Arguments**:
   * Callers passing empty strings or `0` to satisfy fat interface requirements they don't care about.
4. **Heavy Mock Setup in Unit Tests**:
   * Having to stub out 12 virtual methods in a test mock just to test a function that calls 1 method.
