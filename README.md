# Low-Level Design (LLD) & Software Architecture in Modern C++

![Language](https://img.shields.io/badge/Language-C%2B%2B17%20%2F%20C%2B%2B20-00599C?logo=c%2B%2B&logoColor=white)
![Focus](https://img.shields.io/badge/Focus-Low--Level%20Design%20%26%20OOP-2ea44f)
![Principles](https://img.shields.io/badge/Architecture-SOLID%20%7C%20Design%20Patterns-orange)

A structured, code-first engineering repository dedicated to mastering **Object-Oriented Programming (OOP)**, **SOLID Design Principles**, **Design Patterns**, and **Production-Grade Low-Level System Design** in Modern C++.

Rather than treating design principles as abstract textbook definitions, every topic in this repository is explored through a **Problem $\rightarrow$ Violation $\rightarrow$ Refactored Solution $\rightarrow$ Trade-off Analysis** workflow backed by self-contained, runnable C++ implementations.

---

## Why This Repository?

Writing code that *compiles* is easy; designing systems that remain **extensible, testable, and resilient to changing requirements** is the real software engineering challenge.

Each module in this repository is engineered around four pillars:
1. **Architectural Notes (`*Notes.md`)**: Concise revision guides covering design contracts, ASCII architecture diagrams, code smells, and C++ mechanics (`vtable`/`vptr` dispatch, covariant return types, pure interfaces vs. abstract classes, `const`-correctness, and smart pointers).
2. **The Anti-Pattern (`*Violation.cpp`)**: Runnable code demonstrating realistic design flaws—God classes, fragile `switch-case` ladders, broken subtype contracts, and polluted interfaces.
3. **The Refactored Architecture (`*Solution.cpp`)**: Clean, decoupled C++ solutions showing how proper abstractions eliminate blast radius and turn runtime crashes into compile-time guarantees.
4. **Real-World Practice Systems & Trade-offs**: End-to-end mini-systems paired with critical analysis of **when NOT to over-engineer** (e.g., avoiding premature abstraction, YAGNI, and deep inheritance vs. composition).

---

## Repository Structure

```text
lld_with_cpp/
├── foundation concepts/                  # Core OOP pillars & C++ mechanics
│   ├── oopBasics.md                      # Core definitions & mental models
│   ├── classAndObject.cpp                # Object lifecycle, state & behavior
│   ├── encapsulation.cpp                 # Invariant protection & access control
│   ├── inheritance.cpp                   # "Is-a" relationships & access modes
│   ├── composition.cpp                   # "Has-a" component assembly
│   ├── compileTimePolymorphism.cpp       # Overloading & templates
│   ├── runtimePolymorphism.cpp           # Virtual dispatch, vtable/vptr & interfaces
│   ├── staticMemberVariables.cpp         # Class-level state & lifetime
│   └── practice_questions/               # Applied OOP domain problems (Wallet, Student, etc.)
│
├── SOLID principles/                     # Deep-dive into SOLID software architecture
│   ├── S : Single Responsibility Principle/
│   │   ├── srpNotes.md
│   │   ├── srpViolation.cpp & srpSolution.cpp
│   │   └── practice_problems/            # User Auth System, E-Commerce Cart & Checkout Engine
│   │
│   ├── O : Open Close Principle/
│   │   ├── ocpNotes.md & whenNotToUseOCP.md
│   │   ├── ocpViolation.cpp & ocpSolution.cpp
│   │   └── practice_problems/            # Multi-Channel Alert Service, Dynamic Tax & Duty Engine
│   │
│   ├── L : Liskov Substitution/
│   │   ├── lspNotes.md
│   │   ├── lspViolation.cpp & lspSolution.cpp
│   │   ├── lsp_rules/                    # Return type covariance, preconditions & postconditions
│   │   └── practice_questions/           # Document Editor, Shipping Contracts, Delivery Fleet
│   │
│   ├── I : Interface Segration Principle/
│   │   ├── ispNotes.md
│   │   ├── ispViolation.cpp & ispSolution.cpp
│   │   └── practice_problems/            # Smart Home IoT Hub & Role-Segregated Systems
│   │
│   └── D : Dependency Inversion Principle/
│       └── (Upcoming)
│
├── exceptionHandling.cpp                 # Custom exception hierarchies & exception safety
└── progress.md                           # Detailed learning roadmap & topic tracker
```

---

## Key Highlights & Case Studies Implemented

| Module | Real-World Case Studies & Engineering Concepts Covered |
| :--- | :--- |
| **OOP Foundations** | Encapsulation invariants (`Wallet`), composition over inheritance, static vs. dynamic polymorphism, `virtual` destructors, and memory ownership. |
| **S — Single Responsibility (SRP)** | Decoupling domain models, business rules, formatting, and persistence (`Invoice System`, `User Registration Pipeline`, `Shopping Cart & Checkout Orchestrator`). |
| **O — Open/Closed (OCP)** | Replacing fragile conditional ladders with polymorphic strategy contracts (`Payment Gateway`, `Multi-Channel Alert Dispatcher`, `Dynamic Tax Engine`, plus YAGNI trade-offs). |
| **L — Liskov Substitution (LSP)** | Behavioral subtyping vs. taxonomy, formal Design-by-Contract rules (preconditions, postconditions, invariants, covariance), and solving the Diamond Problem via composition (`Delivery Fleet`, `Shipping Providers`). |
| **I — Interface Segregation (ISP)** | Eliminating fat interfaces, designing stateless C++ role interfaces (`IPrinter`, `IScanner`, `Smart Home IoT Hub`), and preventing recompilation cascades. |
| **D — Dependency Inversion (DIP)** | High-level policy decoupling and Dependency Injection *(In Progress)*. |

---

## Getting Started (Compile & Run)

Every `.cpp` file is self-contained with its own `main()` driver so you can compile and run any concept or practice system directly.

```bash
# Clone the repository
git clone https://github.com/<your-username>/lld_with_cpp.git
cd lld_with_cpp

# Compile and run any example (C++20 recommended)
clang++ -std=c++20 "SOLID principles/O : Open Close Principle/ocpSolution.cpp" -o app && ./app
```

---

## Roadmap

- [x] **Phase 1**: Object-Oriented Programming (OOP) Foundations & C++ Mechanics
- [x] **Phase 2**: SOLID Principles (`S`, `O`, `L` Completed; `I`, `D` In Progress)
- [ ] **Phase 3**: Gang of Four (GoF) Design Patterns (Creational, Structural, Behavioral)
- [ ] **Phase 4**: Full-Scale Low-Level System Design Problems (Parking Lot, Elevator, Splitwise, Rate Limiter, etc.)
