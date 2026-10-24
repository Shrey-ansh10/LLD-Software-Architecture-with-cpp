# Single Responsibility Principle (SRP)

> **"A class should have one, and only one, reason to change."**  
> — Robert C. Martin (Uncle Bob)

In simpler terms: **A class should be responsible to one, and only one, 'actor' or 'stakeholder'.**

---

## 1. The Core Idea

When a class takes on multiple unrelated responsibilities, it couples those responsibilities together. A change required by one stakeholder (e.g., Finance wanting a tax calculation change) forces modifications to a class that also handles concerns for another stakeholder (e.g., DBA changing how data is stored, or Frontend changing how invoices are rendered).

### Common Misconception
* **Myth**: "A class should only have one function or do only one tiny task."
* **Reality**: A class can have multiple methods and data members, as long as **all of them are cohesive and serve the same single responsibility / business reason**.

---

## 2. Real-World Scenario: The Invoice System

Consider an e-commerce checkout system where an invoice must be created:
1. **Core Business Logic**: Store line items, calculate total amount, apply taxes, apply discounts. *(Stakeholder: Finance / Accounting)*
2. **Presentation / Formatting**: Print the invoice to the console, format it as HTML, or create a PDF report. *(Stakeholder: UI / Customer Experience)*
3. **Persistence / Storage**: Save the invoice to a database (SQL/NoSQL) or write it to a file. *(Stakeholder: Database Admin / Infrastructure)*

---

## 3. What Happens Without SRP? (`srp_violation.cpp`)

Putting all three responsibilities into a single `Invoice` class creates a **God Object** / **Swiss Army Knife**:

```
+-----------------------------------------------------------+
|                          Invoice                          |
+-----------------------------------------------------------+
| - items: vector<Item>                                     |
| - taxRate: double                                         |
+-----------------------------------------------------------+
| + calculateTotal(): double       --> (Finance Reason)     |
| + printInvoice(): void           --> (UI / Display Reason)|
| + saveToDatabase(): void         --> (Storage Reason)     |
+-----------------------------------------------------------+
```

### Problems & Pain Points:
1. **Multiple Reasons to Change**:
   - Tax rate logic changes? $\rightarrow$ Modify `Invoice`.
   - Management wants a PDF/HTML invoice instead of console print? $\rightarrow$ Modify `Invoice`.
   - Moving from MySQL to MongoDB or writing to a file? $\rightarrow$ Modify `Invoice`.
2. **Blast Radius & Regression Risk**:
   - Changing the database saving logic could inadvertently introduce a bug in the calculation logic or break existing tests.
3. **Team Merge Conflicts**:
   - The database engineer, UI developer, and finance developer all edit `Invoice.cpp` simultaneously.
4. **Testing Nightmare**:
   - To unit test financial calculations, your test must now carry dummy database dependencies or mock file systems.
5. **Lack of Reusability**:
   - You cannot reuse the invoice calculation logic in a mobile app or CLI tool without also dragging in database drivers and specific printing logic.

---

## 4. What Happens With SRP? (`srp_solution.cpp`)

We separate the concerns into distinct, focused classes:

```
+---------------------------------+
|             Invoice             |  <-- Single Responsibility: Business logic & calculations
+---------------------------------+
| - items: vector<Item>           |
| - taxRate: double               |
+---------------------------------+
| + calculateSubtotal(): double   |
| + calculateTotal(): double      |
+---------------------------------+
               │
               ├───────────────────────────────┐
               ▼                               ▼
+------------------------------+  +-------------------------------+
|        InvoicePrinter        |  |       InvoiceRepository       |
+------------------------------+  +-------------------------------+
| + print(invoice: Invoice)    |  | + save(invoice: Invoice)      |
+------------------------------+  +-------------------------------+
  (UI / Formatting only)             (Database / Persistence only)
```

### Key Benefits:
* **High Cohesion & Low Coupling**: Each class does one thing well and has minimal dependencies.
* **Independent Evolutions**: Changing your database engine doesn't touch `Invoice` or `InvoicePrinter`.
* **Easy Unit Testing**: Testing `Invoice::calculateTotal()` is trivial—no databases or printers needed.
* **Extensibility**: Want to print to HTML or JSON? Create `HtmlInvoicePrinter` or `JsonInvoicePrinter` without modifying `Invoice`.

---

## 5. Code Smells & Warning Signs (LLD Interview Tips)

Watch out for these red flags during system design:
* **Monster / God Classes**: Classes with dozens of methods doing calculations, network calls, DB operations, and UI rendering.
* **Mixed Imports / Headers**: An `Invoice.h` file `#include`ing database headers (`<sqlite3.h>`), file I/O (`<fstream>`), and UI libraries.
* **Methods with differing verbs**: `calculate...()`, `render...()`, `sendToNetwork...()`, `saveToDb...()` in the same class.
* **Unstable Unit Tests**: Tests failing for financial logic when database connection configurations change.
