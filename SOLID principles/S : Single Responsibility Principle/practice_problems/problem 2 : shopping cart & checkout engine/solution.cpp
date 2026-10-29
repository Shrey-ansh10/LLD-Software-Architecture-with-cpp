#include <cmath>
#include <iostream>
#include <unordered_map>
using namespace std;

//=======================
// PRODUCT
//=======================
struct Product {
  string productId;
  string productName;
  double productPrice;
};

//=======================
// INVENTORY
//=======================
class Inventory {
  unordered_map<string, int> currentStock;

public:
  bool checkInventory(const string &pid, const int quantity) {
    return (currentStock.contains(pid) && currentStock[pid] >= quantity)
               ? true
               : false;
  }

  void deductStock(const string &pid, const int quantities) {

    if (currentStock.contains(pid) && currentStock[pid] >= quantities) {
      currentStock[pid] -= quantities;
      cout << "Itventory updated." << endl;
    }
  }

  // Update quantity or add new item to inventory stock
  void addStock(const string &pid, int qty) {
    if (qty <= 0) // early exit
      return;

    // increment quantity if stock already exist
    if (currentStock.contains(pid)) {
      currentStock[pid] += qty;
      // cout << "quantity of existing item increased. Now : " <<
      // currentStock[pid] << endl;
    } else {
      // ADD A COMPLETELY NEW ITEM in the inventory.
      currentStock[pid] = qty;
    }
  }

  int getStock(const string &pid) {
    return currentStock.contains(pid) ? currentStock[pid] : 0;
  }
};

//=======================
// CART
//=======================

struct CartItem {
  Product product;
  int quantity;
};

class Cart {
  std::unordered_map<std::string, CartItem> itemsInCart;
  double cartTotalValue;

public:
  Cart() { cartTotalValue = 0; }

  void addItem(const Product p, const int quantity) {

    if (quantity <= 0)
      return;

    // If item already in cart
    if (itemsInCart.contains(p.productId)) {
      cout << "Item already in the cart. Incrementing quantity." << endl;

      itemsInCart[p.productId].quantity += quantity;
      cout << "added " << quantity << " more to cart." << endl;

      cartTotalValue += p.productPrice * quantity;
      cout << "updated cart value = " << cartTotalValue << endl;

    } else {
      // First create a cart item
      CartItem ci;
      ci.product = p;
      ci.quantity = quantity;

      // add cart item to cart
      itemsInCart.insert({p.productId, ci});
      cout << "New item added to cart." << endl;

      // update the cart total
      cartTotalValue += ci.product.productPrice * ci.quantity;
    }
  }

  void removeItem(const string &id) {
    if (itemsInCart.contains(id)) {
      cartTotalValue -=
          (itemsInCart[id].product.productPrice * itemsInCart[id].quantity);
      itemsInCart.erase(id);
    }
    // note the all the quantity of item will be removed from the cart
  }

  vector<CartItem> getItemsInCart() const {
    std::vector<CartItem> cart;

    for (auto it : itemsInCart) {
      cart.push_back(it.second);
    }

    return cart;
  }

  bool isEmpty() const { return itemsInCart.empty() ? true : false; }

  // this will be used to view the card before finally checking out.
  void displayCart() const {

    for (auto it : itemsInCart) {
      cout << "Item Name :" << it.second.product.productName << endl;
      cout << "Item Price : " << it.second.product.productPrice << endl;
      cout << "Item Quantity : " << it.second.quantity << endl;
      cout << "_________________________" << endl;
    }
  }

  double getCartTotal() const { return cartTotalValue; }

  void clearCart() {
    itemsInCart.clear();
    cartTotalValue = 0;
  }
};

//=======================
// DISCOUNT STRATEGY
//=======================
class DiscountStrategy {
public:
  virtual ~DiscountStrategy() = default;
  virtual double applyDiscount(const double total) const = 0;
};

// can create a object of this class for flat X% discount
class PercentageDiscount : public DiscountStrategy {
  double percent;

public:
  PercentageDiscount(double total) : percent(total) {};

  double applyDiscount(const double amount) const override {
    return amount - ((amount * percent) / 100.0);
  }
};

// use the following class to provide flat amount discount.
// i.e. $15 off if cart value is 100+
class FlatDiscount : public DiscountStrategy {
  double discountValue;

public:
  FlatDiscount(double flatDisVal) : discountValue(flatDisVal) {};

  double applyDiscount(const double cartVal) const override {
    if (discountValue > cartVal)
      return cartVal;
    return cartVal >= 100 ? (cartVal - discountValue) : cartVal;
  }
};

// class BOGODiscount : public DiscountStrategy {
// public:
// };

//=======================
// PAYMENT PROCESSOR
//=======================
class PaymentProcessor {
public:
  virtual ~PaymentProcessor() = default;
  virtual bool pay(const double &amount) const = 0;
};

class CardPayment : public PaymentProcessor {
  const double cardNumber;

public:
  CardPayment(const double &crdNum) : cardNumber(crdNum) {};

  bool pay(const double &amount) const override {
    // logic to process payment
    cout << "Payment processes." << endl;
    return true;
  }
};

class Stripe : public PaymentProcessor {
  const double stripeId;

public:
  Stripe(const double &strpId) : stripeId(strpId) {};
  bool pay(const double &amount) const override {
    // logic to process payment
    cout << "Payment processed." << endl;
    return true;
  }
};

//==============================
// RECEIPT PRINTER
//==============================
class Receipt {
public:
  // ReceiptPrinter(Cart &crt) : cart(crt) {};

  // NOTE : Receipt can itself contain a ref to cart object and call the get
  // cart items function of cart, but that's a design choice. I feel we already
  // call that function in checkout service, so we can utilize that and pass it
  // here and make receipt with that.
  virtual ~Receipt() = default;
  virtual void generateReceipt(vector<CartItem> &orderItems, double subtotal,
                               double finalPaid) const = 0;
};

class PaperReceipt : public Receipt {
public:
  void generateReceipt(vector<CartItem> &orderItems, double subtotal,
                       double finalPaid) const override {

    cout << "ORDER SUMMARY : " << endl;

    for (CartItem c : orderItems) {
      cout << "Item Name : " << c.product.productName << endl;
      cout << "Quantity : " << c.quantity << endl;
      cout << "Total Price : " << c.product.productPrice * c.quantity << endl;
      cout << "----------------------------------" << endl;
    }
    cout << "Order Sub Total : " << subtotal << endl;
    cout << "Final amount paid after any discount : " << finalPaid << endl;
    cout << "_____________________________________" << endl;
  }
};

// following can be implemented later
// class EmailReceipt : public Receipt {
// const string &emailId;
// public:
//   EmailReceipt(const string &emailAddress) : emailId(emailAddress) {};
//   void generateReceipt(vector<CartItem>& orderItems) const override {
//     // logic to email the recipt pdf or recipt itself
//   }
// };

//==============================
// ORDER SERVICE - ORCHESTRATOR
//==============================
class OrderService {
  Cart &cart;
  DiscountStrategy &discount;
  PaymentProcessor &pay;
  Inventory &i;
  Receipt &receipt;

public:
  OrderService(Cart &crt, DiscountStrategy &disc, PaymentProcessor &type,
               Inventory &inv, Receipt &r)
      : cart(crt), discount(disc), pay(type), i(inv), receipt(r) {};

  bool checkout() {
    if (!cart.isEmpty()) {

      // cart.displayCart();

      // check all items are in stock before any further step
      vector<CartItem> cartItems = cart.getItemsInCart();
      for (CartItem c : cartItems) {
        if (!i.checkInventory(c.product.productId, c.quantity)) {
          cout << c.product.productName
               << " not enought in stock. Can't proceed." << endl;
          return false;
        }
      }

      // get cart total
      double cartTotal = cart.getCartTotal();
      cout << "Cart total = " << cartTotal << endl;

      // apply respective discount
      double finalPayable = discount.applyDiscount(cartTotal);
      cout << "Final payable amount after discount = " << finalPayable << endl;

      // proceed to pay
      if (pay.pay(finalPayable)) {
        cout << "Payment successful. \n Order Placed" << endl;

        // update inventory - if payment successful
        for (CartItem c : cartItems) {
          i.deductStock(c.product.productId, c.quantity);
        }

        // print receit
        receipt.generateReceipt(cartItems, cartTotal, finalPayable);

        // clear cart - to mitigate problems when user calls the same function
        // more than once.
        cart.clearCart(); // this ensure order isn't placed multiple times and
                          // user isn't twice.

        // exist
        return true;
      } else {
        cout << "payment unsuccesful. \n Retry later." << endl;
        return false;
      }

    } else {
      cout << "cart is empty" << endl;
      return false;
    }
  }
};

// Note : Tests generated by AI
// ============== MINI TEST FRAMEWORK ==============
int passed = 0, failed = 0;
void check(bool condition, const string &name) {
  if (condition) {
    cout << "  [PASS] " << name << endl;
    passed++;
  } else {
    cout << "  [FAIL] " << name << endl;
    failed++;
  }
}
bool almostEqual(double a, double b) { return fabs(a - b) < 0.001; }

// ============== TEST DOUBLES ==============
// Fake payment: never touches a real gateway, just RECORDS what it was asked.
// This is exactly why PaymentProcessor being an interface matters.
class FakePayment : public PaymentProcessor {
  bool shouldSucceed;

public:
  mutable double lastAmountCharged =
      -1; // 'mutable' lets a const method modify it
  mutable int timesCalled = 0;
  FakePayment(bool succeed) : shouldSucceed(succeed) {}
  bool pay(const double &amount) const override {
    lastAmountCharged = amount;
    timesCalled++;
    return shouldSucceed;
  }
};

// Neutral discount so order tests aren't affected by discount logic
class NoDiscount : public DiscountStrategy {
public:
  double applyDiscount(const double total) const override { return total; }
};
const Product KEYBOARD{"P1", "Keyboard", 100.0};
const Product MOUSE{"P2", "Mouse", 25.0};
const Product CABLE{"P3", "Cable", 9.99};

class SpyReceipt : public Receipt {
public:
  mutable int timesGenerated = 0;
  mutable int lastItemCount = 0;
  mutable double lastSubtotal = -1;
  mutable double lastFinalPaid = -1;
  void generateReceipt(vector<CartItem> &orderItems, double subtotal,
                       double finalPaid) const override {
    timesGenerated++;
    lastItemCount = orderItems.size();
    lastSubtotal = subtotal;
    lastFinalPaid = finalPaid;
  }
};

// ============== CART TESTS ==============
void testNewCartIsEmpty() {
  cout << "\nTEST: new cart is empty" << endl;
  Cart cart;
  check(cart.isEmpty(), "isEmpty() is true");
  check(almostEqual(cart.getCartTotal(), 0.0), "total is 0");
}
void testAddSingleItem() {
  cout << "\nTEST: add single item" << endl;
  Cart cart;
  cart.addItem(KEYBOARD, 2);
  check(!cart.isEmpty(), "cart not empty");
  check(almostEqual(cart.getCartTotal(), 200.0), "total is 200");
}
void testAddSameItemTwice() {
  cout << "\nTEST: add same item twice" << endl;
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  cart.addItem(KEYBOARD, 2);
  check(almostEqual(cart.getCartTotal(), 300.0), "total is 300");
  vector<CartItem> items = cart.getItemsInCart();
  check(items.size() == 1, "only 1 line item");
  check(!items.empty() && items[0].quantity == 3, "quantity is 3");
}
void testAddDifferentItems() {
  cout << "\nTEST: add different items" << endl;
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  cart.addItem(MOUSE, 2);
  check(almostEqual(cart.getCartTotal(), 150.0), "total is 150");
  check(cart.getItemsInCart().size() == 2, "2 line items");
}
void testRemoveItem() {
  cout << "\nTEST: remove item" << endl;
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  cart.addItem(MOUSE, 2);
  cart.removeItem("P2");
  check(cart.getItemsInCart().size() == 1, "1 line item left");
  check(almostEqual(cart.getCartTotal(), 100.0), "total is 100");
}
void testRemoveMissingItem() {
  cout << "\nTEST: remove item not in cart" << endl;
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  cart.removeItem("NOPE");
  check(almostEqual(cart.getCartTotal(), 100.0), "total unchanged");
}
void testInvalidQuantity() { // design decision: should these be rejected?
  cout << "\nTEST: zero / negative quantity" << endl;
  Cart cart;
  cart.addItem(MOUSE, 0);
  cart.addItem(MOUSE, -3);
  check(cart.isEmpty(), "cart stays empty");
  check(almostEqual(cart.getCartTotal(), 0.0), "total stays 0");
}

// ============== DISCOUNT TESTS ==============
// Contract: applyDiscount(total) returns the FINAL PAYABLE amount.
void testPercentageDiscount() {
  cout << "\nTEST: 10% discount" << endl;
  PercentageDiscount tenPercent(10);
  check(almostEqual(tenPercent.applyDiscount(200), 180.0), "200 -> 180");
}
void testFlatDiscount() {
  cout << "\nTEST: flat $15 discount (min cart 100)" << endl;
  FlatDiscount flat(15.0);
  check(almostEqual(flat.applyDiscount(200), 185.0), "200 -> 185");
  check(almostEqual(flat.applyDiscount(50), 50.0),
        "50 -> 50 (below threshold)");
}
void testFlatDiscountNeverNegative() {
  cout << "\nTEST: flat discount bigger than cart" << endl;
  FlatDiscount flat(150.0);
  check(flat.applyDiscount(120) >= 0, "payable never negative");
}
// ============== INVENTORY TESTS ==============
void testInventoryCheck() {
  cout << "\nTEST: inventory check" << endl;
  Inventory inv;
  inv.addStock("P1", 5);
  check(inv.checkInventory("P1", 5), "5 of 5 available");
  check(!inv.checkInventory("P1", 6), "6 of 5 not available");
  check(!inv.checkInventory("UNKNOWN", 1), "unknown product not available");
}
void testInventoryDeduct() {
  cout << "\nTEST: inventory deduct" << endl;
  Inventory inv;
  inv.addStock("P1", 5);
  inv.deductStock("P1", 2);
  check(inv.getStock("P1") == 3, "stock is 3");
}
// ============== ORDER SERVICE TESTS ==============
void testCheckoutHappyPath() {
  cout << "\nTEST: checkout happy path" << endl;
  Inventory inv;
  inv.addStock("P1", 10);
  inv.addStock("P2", 10);
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  cart.addItem(MOUSE, 2);
  NoDiscount none;
  FakePayment payment(true);
  PaperReceipt receipt;
  OrderService service(cart, none, payment, inv, receipt);
  check(service.checkout(), "checkout returns true");
  check(almostEqual(payment.lastAmountCharged, 150.0), "charged 150");
  check(inv.getStock("P1") == 9, "keyboard stock 10 -> 9");
  check(inv.getStock("P2") == 8, "mouse stock 10 -> 8");
}
void testCheckoutOutOfStock() {
  cout << "\nTEST: checkout out of stock" << endl;
  Inventory inv;
  inv.addStock("P1", 1);
  Cart cart;
  cart.addItem(KEYBOARD, 2);
  NoDiscount none;
  FakePayment payment(true);
  PaperReceipt receipt;
  OrderService service(cart, none, payment, inv, receipt);
  check(!service.checkout(), "checkout returns false");
  check(payment.timesCalled == 0, "payment never attempted");
  check(inv.getStock("P1") == 1, "stock unchanged");
}
void testCheckoutPaymentFails() {
  cout << "\nTEST: checkout payment fails" << endl;
  Inventory inv;
  inv.addStock("P1", 5);
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  NoDiscount none;
  FakePayment payment(false);
  PaperReceipt receipt;
  OrderService service(cart, none, payment, inv, receipt);
  check(!service.checkout(), "checkout returns false");
  check(inv.getStock("P1") == 5, "stock NOT deducted");
}
void testCheckoutEmptyCart() {
  cout << "\nTEST: checkout empty cart" << endl;
  Inventory inv;
  Cart cart;
  NoDiscount none;
  FakePayment payment(true);
  PaperReceipt receipt;
  OrderService service(cart, none, payment, inv, receipt);
  check(!service.checkout(), "checkout returns false");
  check(payment.timesCalled == 0, "payment never attempted");
}
void testCheckoutWithDiscount() {
  cout << "\nTEST: checkout with 10% discount" << endl;
  Inventory inv;
  inv.addStock("P1", 5);
  inv.addStock("P2", 5);
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  cart.addItem(MOUSE, 2);
  PercentageDiscount tenPercent(10);
  FakePayment payment(true);
  PaperReceipt receipt;
  OrderService service(cart, tenPercent, payment, inv, receipt);
  service.checkout();
  check(almostEqual(payment.lastAmountCharged, 135.0), "charged 135");
}
void testCheckoutFractionalAmount() {
  cout << "\nTEST: checkout with cents" << endl;
  Inventory inv;
  inv.addStock("P3", 5);
  Cart cart;
  cart.addItem(CABLE, 1);
  NoDiscount none;
  FakePayment payment(true);
  PaperReceipt receipt;
  OrderService service(cart, none, payment, inv, receipt);
  service.checkout();
  check(almostEqual(payment.lastAmountCharged, 9.99), "charged 9.99");
}
void testCheckoutTwice() { // design decision: what SHOULD happen?
  cout << "\nTEST: checkout called twice" << endl;
  Inventory inv;
  inv.addStock("P1", 10);
  Cart cart;
  cart.addItem(KEYBOARD, 1);
  NoDiscount none;
  FakePayment payment(true);
  PaperReceipt receipt;
  OrderService service(cart, none, payment, inv, receipt);
  service.checkout();
  service.checkout();
  check(payment.timesCalled == 1, "customer charged only once");
}

// ============== RECEIPT TESTS ==============
void testReceiptOnlyGeneratedOnSuccess() {
  cout << "\nTEST: receipt generated only when order succeeds" << endl;
  Inventory inv;
  inv.addStock("P1", 5);
  Cart cart;
  cart.addItem(KEYBOARD, 2);
  PercentageDiscount tenPercent(10);
  // Case 1: Payment fails -> NO receipt should be generated
  FakePayment failedPay(false);
  SpyReceipt receipt1;
  OrderService failedService(cart, tenPercent, failedPay, inv, receipt1);
  failedService.checkout();
  check(receipt1.timesGenerated == 0, "no receipt when payment fails");
  // Case 2: Payment succeeds -> Receipt generated once with subtotal &
  // finalPaid
  FakePayment okPay(true);
  SpyReceipt receipt2;
  OrderService okService(cart, tenPercent, okPay, inv, receipt2);
  okService.checkout();
  check(receipt2.timesGenerated == 1, "receipt generated once on success");
  check(receipt2.lastItemCount == 1, "receipt received the cart items");
  check(almostEqual(receipt2.lastSubtotal, 200.0), "receipt subtotal is 200");
  check(almostEqual(receipt2.lastFinalPaid, 180.0),
        "receipt final paid is 180");
}

int main() {
  testNewCartIsEmpty();
  testAddSingleItem();
  testAddSameItemTwice();
  testAddDifferentItems();
  testRemoveItem();
  testRemoveMissingItem();
  testInvalidQuantity();
  testPercentageDiscount();
  testFlatDiscount();
  testFlatDiscountNeverNegative();
  testInventoryCheck();
  testInventoryDeduct();
  testCheckoutHappyPath();
  testCheckoutOutOfStock();
  testCheckoutPaymentFails();
  testCheckoutEmptyCart();
  testCheckoutWithDiscount();
  testCheckoutFractionalAmount();
  testCheckoutTwice();
  testReceiptOnlyGeneratedOnSuccess();
  cout << "\n=============================" << endl;
  cout << "Passed: " << passed << "   Failed: " << failed << endl;
  return 0;
}
