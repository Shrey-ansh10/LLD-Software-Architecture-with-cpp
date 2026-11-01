# Trade-offs & Critical Thinking: When NOT to Use OCP

> **"Premature abstraction is just as harmful as no abstraction."**

While the Open/Closed Principle (OCP) is essential for extensible design, applying it blindly to **every** class in a system leads to **Over-Engineering** (also known as *Speculative Generality*).

---

## 1. The Cost of Applying OCP Everywhere Prematurely

If a team forces every class to have an abstract interface "just in case":

1. **Class Explosion & Boilerplate (`IThing` + `ThingImpl`)**: ("I" stands for Interface)
   * Every simple concept turns into multiple classes (`IShoppingCart` + `DefaultShoppingCart`, `IProduct` + `StandardProduct`).
   * The codebase doubles in size without adding any real business capability.

2. **Indirection & Reduced Readability (Navigation Tax)**:
   * Every interface adds a layer of indirection.
   * When reading or debugging code, jumping to the definition of a method takes you to a pure virtual declaration (`virtual void doWork() = 0;`) rather than the actual logic. Maintainers must constantly trace which concrete class is wired up at runtime.

3. **The Wrong Abstraction Problem**:
   * Predicting the future is hard. If you design an abstract interface *before* you have a second concrete use case, you will likely guess the method parameters and contract wrong.
   * When a real requirement finally arrives, it won't fit your guessed interface—forcing you to modify the interface and every class anyway.

4. **C++ Specific Cost (Virtual Dispatch Overhead)**:
   * In C++, `virtual` functions require a `vptr`/`vtable` lookup at runtime and often prevent the compiler from **inlining** small, frequently called functions. While negligible for I/O or business services, it matters in performance-critical inner loops.

---

## 2. How to Decide: When to Use OCP vs. Keep It Simple

### Rule 1: Identify the "Axis of Variation" (Your Core Question)
Ask yourself: **"Will there genuinely be multiple variants/implementations of this behavior?"**

| Keep It Simple (Concrete Class) | Apply OCP (Abstract Interface) |
| :--- | :--- |
| **Domain Entities / Data Models**: `User`, `Product`, `CartItem`, `Invoice` | **Business Rules & Policies**: `DiscountStrategy`, `TaxRule` |
| **Core State Containers**: `ShoppingCart` (there is only one way to hold cart items) | **External Integrations / Gateways**: `PaymentMethod` (Stripe, UPI, PayPal) |
| **Internal Helper Utilities**: String/Math helpers, simple calculators | **I/O & Delivery Channels**: `NotificationChannel` (Email, SMS, Slack), `Repository` |

> **Example from our Shopping Cart Design**:  
> `ShoppingCart` itself does **not** need an abstract interface (`IShoppingCart`) because a cart simply holds items. However, `PaymentMethod` and `DiscountStrategy` **do** need interfaces because business rules and payment providers vary constantly.

---

### Rule 2: YAGNI ("You Aren't Gonna Need It") & "Fool Me Once"
Even Robert C. Martin (Uncle Bob) warns against abstracting everything on Day 1. Instead, follow the **"Fool Me Once" principle**:

1. **1st Variant**: If there is only one way something works today and no concrete requirement for a second way, write a **simple concrete class**.
2. **2nd Variant Arrives**: The moment a second variant is requested (e.g., *"We have Email alerts, now add SMS"*), **refactor** the code into an abstract interface (`NotificationChannel`).
3. **3rd+ Variants**: Now your system is open for extension (`Slack`, `Discord`, `WhatsApp` drop right in).

---

### Rule 3: Abstract at System Boundaries
Always lean toward interfaces when crossing boundaries outside your core application:
* **Databases** (SQL, NoSQL, In-Memory fake for unit testing)
* **Third-party APIs** (Payment gateways, SMS providers)
* **File formats / Parsers** (CSV, JSON, XML)

Keep internal domain objects (`User`, `Order`, `Cart`) concrete and direct.

