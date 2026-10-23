/**
 * Single Responsibility Principle (SRP) - VIOLATION EXAMPLE
 * 
 * "A class should have one, and only one, reason to change."
 * 
 * Problem in this file:
 * The `Invoice` class takes on THREE distinct responsibilities:
 * 1. Business Logic & Calculations (calculate total, tax, discount) -> Reason to change: Finance team
 * 2. Printing & Formatting (print invoice details to console)       -> Reason to change: UI/UX team
 * 3. Persistence / Data Storage (saving invoice to a file / DB)     -> Reason to change: Database/Infra team
 * 
 * Because these three concerns are bundled together, any change to one
 * can inadvertently break the others, complicates testing, and creates merge conflicts.
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

struct Item {
    std::string name;
    double price;
    int quantity;

    Item(const std::string& n, double p, int q)
        : name(n), price(p), quantity(q) {}
};

class Invoice {
private:
    std::string invoiceId;
    std::vector<Item> items;
    double taxRate; // e.g., 0.10 for 10%

public:
    Invoice(const std::string& id, double tax)
        : invoiceId(id), taxRate(tax) {}

    void addItem(const Item& item) {
        items.push_back(item);
    }

    // =========================================================================
    // RESPONSIBILITY 1: Business Logic & Financial Calculations
    // Reason to change: Finance or business rules change (e.g., discounts, tiered tax)
    // =========================================================================
    double calculateSubtotal() const {
        double subtotal = 0.0;
        for (const auto& item : items) {
            subtotal += item.price * item.quantity;
        }
        return subtotal;
    }

    double calculateTotal() const {
        double subtotal = calculateSubtotal();
        return subtotal + (subtotal * taxRate);
    }

    // =========================================================================
    // RESPONSIBILITY 2: Presentation & Formatting
    // Reason to change: Customer wants a different format (PDF, HTML, currency symbol, layout)
    // VIOLATION: Why should Invoice class care about HOW it is displayed?
    // =========================================================================
    void printInvoice() const {
        std::cout << "================ INVOICE ================\n";
        std::cout << "Invoice ID: " << invoiceId << "\n";
        std::cout << "-----------------------------------------\n";
        for (const auto& item : items) {
            std::cout << item.name << " x " << item.quantity 
                      << " @ $" << std::fixed << std::setprecision(2) << item.price
                      << " = $" << (item.price * item.quantity) << "\n";
        }
        std::cout << "-----------------------------------------\n";
        std::cout << "Subtotal : $" << calculateSubtotal() << "\n";
        std::cout << "Tax Rate : " << (taxRate * 100) << "%\n";
        std::cout << "Total    : $" << calculateTotal() << "\n";
        std::cout << "=========================================\n";
    }

    // =========================================================================
    // RESPONSIBILITY 3: Persistence & Storage
    // Reason to change: Switching from file storage to SQL/NoSQL database, schema change
    // VIOLATION: Why should Invoice class know about databases or file systems?
    // =========================================================================
    void saveToFile() const {
        // Simulating saving invoice to disk / database
        std::cout << "[Storage] Saving Invoice #" << invoiceId << " to invoice_records.txt...\n";
        std::cout << "[Storage] Invoice #" << invoiceId << " saved successfully.\n";
    }
};

int main() {
    Invoice invoice("INV-2026-001", 0.18); // 18% GST/tax
    invoice.addItem(Item("Mechanical Keyboard", 85.00, 1));
    invoice.addItem(Item("Wireless Mouse", 35.00, 2));

    // Notice how one single object is responsible for calculations, display, and storage:
    invoice.printInvoice();
    invoice.saveToFile();

    return 0;
}
