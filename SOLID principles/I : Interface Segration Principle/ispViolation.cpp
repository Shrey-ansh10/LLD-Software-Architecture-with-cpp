/**
 * Interface Segregation Principle (ISP) - VIOLATION EXAMPLE
 * 
 * "Clients should not be forced to depend upon interfaces that they do not use."
 * 
 * Problem in this file:
 * `IMultiFunctionDevice` is a "Fat Interface" that bundles printing, scanning,
 * and faxing into a single contract.
 * 
 * CONSEQUENCES:
 * 1. Implementer Pain: `BasicInkjetPrinter` only supports printing, yet it is FORCED
 *    to implement `scan()` and `fax()` (leading to dummy code or exceptions -> LSP Violation!).
 * 2. Client Coupling: `printMonthlyReport()` ONLY needs to print, yet it depends on the
 *    entire `IMultiFunctionDevice` interface (including fax and scan signatures).
 */

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// =============================================================================
// FAT INTERFACE (Violates ISP)
// Bundles three distinct capabilities that don't always go together.
// =============================================================================
class IMultiFunctionDevice {
public:
    virtual ~IMultiFunctionDevice() = default;

    virtual void print(const string& document) const = 0;
    virtual void scan(const string& document) const = 0;
    virtual void fax(const string& document, const string& faxNumber) const = 0;
};

// =============================================================================
// IMPLEMENTER 1: High-end machine (Happens to support everything)
// =============================================================================
class EnterpriseLaserJet : public IMultiFunctionDevice {
public:
    void print(const string& document) const override {
        cout << "[EnterpriseLaserJet] Printing: " << document << "\n";
    }

    void scan(const string& document) const override {
        cout << "[EnterpriseLaserJet] Scanning: " << document << " to PDF.\n";
    }

    void fax(const string& document, const string& faxNumber) const override {
        cout << "[EnterpriseLaserJet] Faxing: " << document << " to " << faxNumber << "\n";
    }
};

// =============================================================================
// IMPLEMENTER 2: Simple Home Printer (Forced to implement methods it CANNOT do!)
// =============================================================================
class BasicInkjetPrinter : public IMultiFunctionDevice {
public:
    void print(const string& document) const override {
        cout << "[BasicInkjetPrinter] Printing: " << document << "\n";
    }

    // VIOLATION: Basic printer has no scanner hardware!
    void scan(const string& /*document*/) const override {
        throw logic_error("ISP/LSP Violation: BasicInkjetPrinter cannot scan!");
    }

    // VIOLATION: Basic printer has no fax modem!
    void fax(const string& /*document*/, const string& /*faxNumber*/) const override {
        throw logic_error("ISP/LSP Violation: BasicInkjetPrinter cannot fax!");
    }
};

// =============================================================================
// CLIENT: Only cares about printing, but is coupled to the fat interface
// =============================================================================
void printBillingReport(const IMultiFunctionDevice& device, const string& report) {
    // Even though this client only prints, if `fax()` signature changes in
    // IMultiFunctionDevice, this client must be recompiled!
    device.print(report);
}

int main() {
    EnterpriseLaserJet officeMachine;
    BasicInkjetPrinter homePrinter;

    printBillingReport(officeMachine, "Q4_Financials.pdf");
    printBillingReport(homePrinter, "Grocery_List.txt");

    // Danger waiting at runtime because BasicInkjetPrinter was forced to implement scan():
    try {
        homePrinter.scan("Passport.jpg");
    } catch (const exception& e) {
        cout << "[CRASH] " << e.what() << "\n";
    }

    return 0;
}
