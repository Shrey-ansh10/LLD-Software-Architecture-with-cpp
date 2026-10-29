/**
 * Open / Closed Principle (OCP) - VIOLATION EXAMPLE
 *
 * "Software entities should be open for extension, but closed for
 * modification."
 *
 * Problem in this file:
 * The `PaymentProcessor` uses a conditional `switch-case` (or `if-else`) to
 * inspect the payment type and execute the corresponding payment logic.
 *
 * WHY IS THIS A VIOLATION?
 * Suppose business expands to India and wants to support `UPI` (Google
 * Pay/PhonePe) or `Cryptocurrency`:
 * 1. We MUST modify `enum class PaymentType` to add new constants.
 * 2. We MUST modify `PaymentProcessor::processPayment()` by adding new `case`
 * statements.
 * 3. We have to re-compile `PaymentProcessor` and all files that depend on it.
 * 4. We risk introducing regressions into already-tested Credit Card and PayPal
 * paths.
 *
 * Every new feature requires MODIFICATION of existing, tested code!
 */

#include <iostream>
#include <string>

enum class PaymentType {
  CREDIT_CARD,
  PAYPAL
  // To add UPI or Crypto: WE MUST MODIFY THIS ENUM!
  // , UPI
  // , CRYPTO
};

class PaymentProcessor {
public:
  void processPayment(PaymentType type, double amount,
                      const std::string &accountDetails) {
    switch (type) {
    case PaymentType::CREDIT_CARD: {
      // Low-level credit card processing logic
      std::cout << "[Credit Card] Validating card details: " << accountDetails
                << "\n";
      std::cout << "[Credit Card] Contacting Banking Gateway...\n";
      std::cout << "[Credit Card] Successfully charged $" << amount << "\n\n";
      break;
    }

    case PaymentType::PAYPAL: {
      // Low-level PayPal API logic
      std::cout << "[PayPal] Authenticating PayPal user: " << accountDetails
                << "\n";
      std::cout << "[PayPal] Calling PayPal REST API...\n";
      std::cout << "[PayPal] Successfully transferred $" << amount << "\n\n";
      break;
    }

      // =================================================================
      // THE PROBLEM:
      // Whenever a new payment method (UPI, Crypto, ApplePay) is needed:
      // We are forced to OPEN THIS FILE and MODIFY THIS METHOD:
      //
      // case PaymentType::UPI: {
      //     // UPI logic...
      //     break;
      // }
      //
      // This is direct violation of the "Closed for Modification" rule.
      // =================================================================

    default:
      std::cout << "[Error] Unsupported payment method!\n\n";
      break;
    }
  }
};

int main() {
  PaymentProcessor processor;

  std::cout << "--- Processing Existing Payments ---\n";
  processor.processPayment(PaymentType::CREDIT_CARD, 150.00,
                           "4111-2222-3333-4444");
  processor.processPayment(PaymentType::PAYPAL, 75.50, "user@example.com");

  // Imagine the business asks: "Can we accept UPI payments now?"
  // Developer says: "Hold on, I have to open the PaymentProcessor class,
  // modify the enum, modify the switch statement, and hope I don't break Credit
  // Card logic!"

  return 0;
}
