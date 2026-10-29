/*

=> Shopping Cart & Checkout Engine

The Scenario : In an e-commerce platform, the ShoppingCart class has grown out
of control. It currently:

  - Stores the list of products and quantities (addItem, removeItem). - CART
  - Calculates discounts (e.g., 10% off for festival sale, BOGO offers). -
DISCOUNT LOGIC
  - Checks warehouse inventory to see if items are in stock. - COMMUNICATES WITH
INVENTORY
  - Charges the customer's credit card via a payment gateway. - PAYMENT GATEWAY
  - Prints the order summary receipt to the console. - BILLING DISPLAY/PRINT
*/

#include <iostream>
using namespace std;

class ShoppingCart {
  // items list...
public:
  void addItem(Product p, int qty);
  void removeItem(string productId);

  double calculateFinalAmountWithDiscounts(); // Business / Pricing rule

  bool verifyInventoryStock(); // Warehouse / Inventory system

  bool processPayment(string cardNumber); // Payment gateway integration

  void printReceipt(); // UI / Presentation
};

/*

What’s Wrong? (Code Smells)
  - A shopping cart should only be a container for items the user wants to buy.
  - Tying payment processing, inventory checks, and discount logic into
  ShoppingCart makes unit testing impossible without hitting a real payment
  gateway and inventory DB.


=> Your Goal

Refactor this system:
- ShoppingCart: Solely manages cart items and total calculation.
- DiscountStrategy / PricingService: Calculates discounts without polluting the
cart.
- InventoryService: Checks item stock.
- PaymentProcessor: Manages charging the customer.
- OrderService: Orchestrates the checkout flow cleanly by coordinating the
components.

*/