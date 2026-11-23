/**
 * Interface Segregation Principle (ISP) - SOLUTION EXAMPLE
 * 
 * "Clients should not be forced to depend upon interfaces that they do not use."
 * 
 * Solution in this file:
 * 1. Break the fat `IMultiFunctionDevice` interface into small, cohesive "Role Interfaces":
 *    - `IPrinter`
 *    - `IScanner`
 *    - `IFax`
 * 
 * 2. Hardware classes only implement the exact interfaces they genuinely support:
 *    - `BasicInkjetPrinter`  -> implements `IPrinter`
 *    - `OfficePhotocopier`   -> implements `IPrinter`, `IScanner`
 *    - `EnterpriseLaserJet`  -> implements `IPrinter`, `IScanner`, `IFax`
 * 
 * 3. Client functions only ask for the narrow interface they actually need:
 *    - `printBillingReport(const IPrinter& printer, ...)`
 */

#include <iostream>
#include <string>
using namespace std;

// =============================================================================
// SEGREGATED ROLE INTERFACES (Small, focused contracts)
// =============================================================================
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const string& document) const = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const string& document) const = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax(const string& document, const string& faxNumber) const = 0;
};

// =============================================================================
// IMPLEMENTER 1: Basic Printer (Only implements IPrinter — zero dummy methods!)
// =============================================================================
class BasicInkjetPrinter : public IPrinter {
public:
    void print(const string& document) const override {
        cout << "[BasicInkjetPrinter] Printing: " << document << "\n";
    }
};

// =============================================================================
// IMPLEMENTER 2: Office Photocopier (Prints + Scans, no Fax)
// =============================================================================
class OfficePhotocopier : public IPrinter, public IScanner {
public:
    void print(const string& document) const override {
        cout << "[OfficePhotocopier] Printing copy of: " << document << "\n";
    }

    void scan(const string& document) const override {
        cout << "[OfficePhotocopier] Scanning: " << document << "\n";
    }

    // Can also combine its capabilities into a photocopy action:
    void photocopy(const string& document) const {
        cout << "--- Photocopying " << document << " ---\n";
        scan(document);
        print(document);
    }
};

// =============================================================================
// IMPLEMENTER 3: Enterprise All-In-One (Implements all three role interfaces)
// =============================================================================
class EnterpriseLaserJet : public IPrinter, public IScanner, public IFax {
public:
    void print(const string& document) const override {
        cout << "[EnterpriseLaserJet] High-speed printing: " << document << "\n";
    }

    void scan(const string& document) const override {
        cout << "[EnterpriseLaserJet] High-res scanning: " << document << " to PDF.\n";
    }

    void fax(const string& document, const string& faxNumber) const override {
        cout << "[EnterpriseLaserJet] Faxing: " << document << " to " << faxNumber << "\n";
    }
};

// =============================================================================
// CLIENTS (Each client depends ONLY on the narrow interface it actually uses!)
// =============================================================================
void printBillingReport(const IPrinter& printer, const string& report) {
    // Accepts BasicInkjetPrinter, OfficePhotocopier, OR EnterpriseLaserJet!
    // Completely decoupled from scanning or faxing changes.
    printer.print(report);
}

void archiveDocumentToDigital(const IScanner& scanner, const string& doc) {
    // Accepts OfficePhotocopier OR EnterpriseLaserJet!
    // Passing BasicInkjetPrinter here is a COMPILE-TIME error, not a runtime crash.
    scanner.scan(doc);
}

int main() {
    BasicInkjetPrinter homePrinter;
    OfficePhotocopier copier;
    EnterpriseLaserJet enterpriseMachine;

    cout << "=== 1. Printing Client (Accepts any IPrinter) ===\n";
    printBillingReport(homePrinter, "Grocery_List.txt");
    printBillingReport(copier, "Meeting_Handout.pdf");
    printBillingReport(enterpriseMachine, "Q4_Financials.pdf");

    cout << "\n=== 2. Scanning Client (Accepts any IScanner) ===\n";
    archiveDocumentToDigital(copier, "Signed_Receipt.png");
    archiveDocumentToDigital(enterpriseMachine, "Tax_Return.pdf");
    // archiveDocumentToDigital(homePrinter, "Passport.jpg"); // <-- Won't compile! Caught safely at compile time.

    cout << "\n=== 3. Multi-Capability Machine Actions ===\n";
    copier.photocopy("ID_Card.png");
    enterpriseMachine.fax("Legal_Contract.pdf", "+1-800-555-0199");

    return 0;
}
