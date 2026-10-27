/**
    Single Responsibility Principle (SRP) - SOLUTION EXAMPLE

    Solution in this file:
    We decouple the three responsibilities into three distinct, cohesive
   classes:
    1. `Invoice`: ONLY holds invoice data and calculates financial values
   (Subtotal, Total, Taxes).
       -> Reason to change: Business/Finance rules change.

    2. `InvoicePrinter`: ONLY handles formatting and displaying the invoice.
       -> Reason to change: Presentation/UI format changes (console, HTML, PDF).

    3. `InvoiceRepository` (and implementations like `FileInvoiceRepository`):
       ONLY handles saving and loading invoices to persistent storage.
       -> Reason to change: Storage mechanism or database schema changes.

    Now, each class is completely decoupled, easy to test, and can evolve
   independently!
 */

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// =============================================================================
// DOMAIN MODEL: Item
// =============================================================================
struct Item {
  std::string name;
  double price;
  int quantity;

  Item(const std::string &n, double p, int q)
      : name(n), price(p), quantity(q) {}
};

// =============================================================================
// CLASS 1: Invoice (Core Business Entity & Calculation)
// Single Responsibility: Manage invoice state and perform financial
// calculations.
// =============================================================================
class Invoice {
private:
  std::string invoiceId;
  std::vector<Item> items;
  double taxRate;

public:
  Invoice(const std::string &id, double tax) : invoiceId(id), taxRate(tax) {}

  void addItem(const Item &item) { items.push_back(item); }

  const std::string &getId() const { return invoiceId; }
  const std::vector<Item> &getItems() const { return items; }
  double getTaxRate() const { return taxRate; }

  double calculateSubtotal() const {
    double subtotal = 0.0;
    for (const auto &item : items) {
      subtotal += item.price * item.quantity;
    }
    return subtotal;
  }

  double calculateTotal() const {
    double subtotal = calculateSubtotal();
    return subtotal + (subtotal * taxRate);
  }
};

// =============================================================================
// CLASS 2: InvoicePrinter (Presentation / Formatting)
// Single Responsibility: Format and display an invoice.
// Notice: Invoice class doesn't know or care about this class!
// =============================================================================
class InvoicePrinter {
public:
  void printToConsole(const Invoice &invoice) const {
    std::cout << "================ INVOICE ================\n";
    std::cout << "Invoice ID: " << invoice.getId() << "\n";
    std::cout << "-----------------------------------------\n";
    for (const auto &item : invoice.getItems()) {
      std::cout << item.name << " x " << item.quantity << " @ $" << std::fixed
                << std::setprecision(2) << item.price << " = $"
                << (item.price * item.quantity) << "\n";
    }
    std::cout << "-----------------------------------------\n";
    std::cout << "Subtotal : $" << invoice.calculateSubtotal() << "\n";
    std::cout << "Tax Rate : " << (invoice.getTaxRate() * 100) << "%\n";
    std::cout << "Total    : $" << invoice.calculateTotal() << "\n";
    std::cout << "=========================================\n";
  }

  // If tomorrow the marketing team wants HTML output, we can add printToHtml()
  // here or create an HtmlInvoicePrinter without touching the Invoice class!
};

// =============================================================================
// CLASS 3: InvoiceRepository (Persistence / Storage)
// Single Responsibility: Persist and retrieve invoice data.
// =============================================================================
class InvoiceRepository {
public:
  virtual ~InvoiceRepository() = default;
  virtual void save(const Invoice &invoice) = 0;
};

class FileInvoiceRepository : public InvoiceRepository {
public:
  void save(const Invoice &invoice) override {
    // Simulating writing to file storage
    std::cout << "[FileStorage] Writing Invoice #" << invoice.getId()
              << " to disk (invoices.txt)...\n";
    std::cout << "[FileStorage] Total amount logged: $"
              << invoice.calculateTotal() << "\n";
    std::cout << "[FileStorage] Saved successfully.\n";
  }
};

class DatabaseInvoiceRepository : public InvoiceRepository {
public:
  void save(const Invoice &invoice) override {
    // Simulating SQL database insert
    std::cout << "[DBStorage] Executing: INSERT INTO invoices VALUES ('"
              << invoice.getId() << "', " << invoice.calculateTotal() << ");\n";
    std::cout << "[DBStorage] Saved to DB successfully.\n";
  }
};

int main() {
  // 1. Business Logic / Entity
  Invoice invoice("INV-2026-001", 0.18);
  invoice.addItem(Item("Mechanical Keyboard", 85.00, 1));
  invoice.addItem(Item("Wireless Mouse", 35.00, 2));

  // 2. Presentation Responsibility
  InvoicePrinter printer;
  printer.printToConsole(invoice);

  // 3. Storage Responsibility
  std::unique_ptr<InvoiceRepository> repo =
      std::make_unique<DatabaseInvoiceRepository>();
  repo->save(invoice);

  return 0;
}
