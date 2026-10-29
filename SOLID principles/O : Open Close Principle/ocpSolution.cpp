/**
 * Open / Closed Principle (OCP) - SOLUTION EXAMPLE
 * 
 * "Software entities should be open for extension, but closed for modification."
 * 
 * Solution in this file:
 * 1. We create an abstract interface `PaymentMethod` that defines the contract: `pay(double amount)`.
 * 2. Each concrete payment mechanism implements this interface:
 *    - `CreditCardPayment`
 *    - `PayPalPayment`
 *    - `UpiPayment`
 *    - `CryptoPayment`
 * 3. `PaymentProcessor` depends ONLY on the abstraction `PaymentMethod`.
 * 
 * RESULT:
 * - OPEN FOR EXTENSION: Want to support Apple Pay or Bitcoin?
 *   Simply create a new class `ApplePayPayment` or `BitcoinPayment` implementing `PaymentMethod`.
 * - CLOSED FOR MODIFICATION:
 *   `PaymentProcessor` is completely untouched! No switch cases, no enums modified,
 *   zero risk of breaking existing payment paths.
 */

#include <iostream>
#include <string>
#include <memory>

// =============================================================================
// ABSTRACTION (The Contract)
// =============================================================================
class PaymentMethod {
public:
    virtual ~PaymentMethod() = default;
    virtual bool pay(double amount) const = 0;
};

// =============================================================================
// CONCRETE IMPLEMENTATION 1: Credit Card
// =============================================================================
class CreditCardPayment : public PaymentMethod {
private:
    std::string cardNumber;
    std::string cvv;

public:
    CreditCardPayment(const std::string& card, const std::string& c)
        : cardNumber(card), cvv(c) {}

    bool pay(double amount) const override {
        std::cout << "[Credit Card] Validating Card: " << cardNumber << "\n";
        std::cout << "[Credit Card] Charging $" << amount << " via Bank Gateway.\n";
        std::cout << "[Credit Card] Payment Successful!\n\n";
        return true;
    }
};

// =============================================================================
// CONCRETE IMPLEMENTATION 2: PayPal
// =============================================================================
class PayPalPayment : public PaymentMethod {
private:
    std::string email;

public:
    explicit PayPalPayment(const std::string& em) : email(em) {}

    bool pay(double amount) const override {
        std::cout << "[PayPal] Authenticating account: " << email << "\n";
        std::cout << "[PayPal] Transferring $" << amount << " via PayPal REST API.\n";
        std::cout << "[PayPal] Payment Successful!\n\n";
        return true;
    }
};

// =============================================================================
// NEW EXTENSION: UPI Payment (Added later as a new requirement)
// NOTICE: Added WITHOUT modifying PaymentMethod, PaymentProcessor, or CreditCard!
// =============================================================================
class UpiPayment : public PaymentMethod {
private:
    std::string upiId; // e.g. "sarthak@okhdfcbank"

public:
    explicit UpiPayment(const std::string& id) : upiId(id) {}

    bool pay(double amount) const override {
        std::cout << "[UPI] Requesting approval from VPA: " << upiId << "\n";
        std::cout << "[UPI] Debiting $" << amount << " via NPCI network.\n";
        std::cout << "[UPI] Payment Successful!\n\n";
        return true;
    }
};

// =============================================================================
// NEW EXTENSION: Crypto Payment (Added later as another requirement)
// NOTICE: Purely an extension — zero existing classes were modified!
// =============================================================================
class CryptoPayment : public PaymentMethod {
private:
    std::string walletAddress;

public:
    explicit CryptoPayment(const std::string& wallet) : walletAddress(wallet) {}

    bool pay(double amount) const override {
        std::cout << "[Crypto] Broadcasting transaction to blockchain for wallet: " 
                  << walletAddress << "\n";
        std::cout << "[Crypto] Transferred $" << amount << " worth of crypto.\n";
        std::cout << "[Crypto] Transaction Confirmed!\n\n";
        return true;
    }
};

// =============================================================================
// THE PROCESSOR (Closed for modification)
// Depends ONLY on the abstraction `PaymentMethod`.
// =============================================================================
class PaymentProcessor {
public:
    // This function NEVER needs to change when a 5th or 6th payment method is invented!
    bool processPayment(const PaymentMethod& method, double amount) {
        if (amount <= 0) {
            std::cout << "[Error] Invalid payment amount: $" << amount << "\n";
            return false;
        }

        std::cout << ">>> Initiating transaction of $" << amount << " <<<\n";
        return method.pay(amount);
    }
};

// =============================================================================
// MAIN DRIVER
// =============================================================================
int main() {
    PaymentProcessor processor;

    // 1. Pay with Credit Card
    CreditCardPayment cc("4111-2222-3333-4444", "123");
    processor.processPayment(cc, 250.00);

    // 2. Pay with PayPal
    PayPalPayment paypal("user@example.com");
    processor.processPayment(paypal, 120.00);

    // 3. Pay with UPI (Newly extended without altering existing code!)
    UpiPayment upi("developer@upi");
    processor.processPayment(upi, 50.00);

    // 4. Pay with Crypto (Another extension, zero core code touched!)
    CryptoPayment crypto("0x71C...B29");
    processor.processPayment(crypto, 890.00);

    return 0;
}

