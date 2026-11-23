# Interface Segregation Principle (ISP)

> **"Clients should not be forced to depend upon interfaces that they do not use."**  
> — Robert C. Martin (Uncle Bob)

In simpler terms: **Many small, role-specific interfaces are better than one large, general-purpose ("fat") interface.**

---

## 1. The Core Idea: Two Sides of a "Fat Interface"

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
