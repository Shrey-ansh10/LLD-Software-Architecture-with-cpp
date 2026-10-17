#include <iostream>
#include <memory> // this header provides smart pointer
using namespace std;

// Runtime / dynamic polymorphism:
// Same call (s->area()) runs DIFFERENT code depending on the actual object
// sitting behind a base pointer/reference. Needs: inheritance + virtual.

class Shape {
public:
    virtual double area() const = 0; // pure virtual → Shape is abstract (an interface)

    // virtual double area() const = 0
    //    ↑              ↑     ↑
    //    │              │     │
    //    │              │     └─ pure virtual → makes Shape abstract
    //    │              │
    //    │              └─── function doesn't modify object
    //    │
    //    └─── enable runtime polymorphism


    virtual void printType() const {
        cout << "Shape\n";
    }

    

    // Always give polymorphic bases a virtual destructor.
    // Otherwise delete through Shape* can skip the derived destructor.
    virtual ~Shape() = default;
};

class Rectangle : public Shape {
    double length;
    double breadth;

public:
    Rectangle(double l, double b) : length(l), breadth(b) {}

    double area() const override {
        return length * breadth;
    }
//   double area() const override
//    ↑      ↑      ↑      ↑
//    │      │      │      │
//   return  │      │      └─ compiler: verify I'm overriding
//   type    │      │
//           │      └─── I won't modify this object's state
//           │
//           └── function name

    void printType() const override {
        cout << "Rectangle\n";
    }
};

class Square : public Shape {
    double side;

public:
    Square(double s) : side(s) {}

    double area() const override {
        return side * side;
    }

    void printType() const override {
        cout << "Square\n";
    }
};

// Contrast: WITHOUT virtual, the compiler binds the call using the POINTER type,
// not the real object type.
class Base {
public:
    void greet() const { cout << "Base\n"; }           // not virtual
    virtual void speak() const { cout << "Base speak\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void greet() const { cout << "Derived\n"; }        // hides Base::greet, does not override
    void speak() const override { cout << "Derived speak\n"; }
};

int main() {
    Rectangle r(10, 20); // stack: default choice
    cout << r.area() << '\n';

    // Polymorphism does not require new. A base REFERENCE is enough.
    Shape& ref = r;
    cout << ref.area() << '\n';

    // Dynamic + interface: unique_ptr<Shape> can own any kind of Shape.
    unique_ptr<Shape> s1 = make_unique<Rectangle>(10, 20);
    unique_ptr<Shape> s2 = make_unique<Square>(5);

    // static type of s1 is Shape*, dynamic type is Rectangle
    cout << s1->area() << '\n';      // Rectangle's area (virtual)
    cout << s2->area() << '\n';      // Square's area
    s1->printType();
    s2->printType();

    unique_ptr<Base> b = make_unique<Derived>();
    b->greet(); // Base     — no virtual, uses pointer type
    b->speak(); // Derived speak — virtual, uses actual object
    
    return 0;
}


/*
Rectangle r(...)
    → ordinary local object
    → simplest/default lifetime

unique_ptr<Shape>
    → dynamically allocated object
    → owned safely

Shape pointer/interface
    → polymorphism

virtual
    → dynamic dispatch enabled

override
    → compiler verifies derived implementation

virtual destructor
    → safe polymorphic destruction


                    Creating an object
                           │
             ┌─────────────┴──────────────┐
             │                            │
       Normal/local                 Dynamic lifetime
             │                            │
             ▼                            ▼
 Rectangle r(10,20);         make_unique<Rectangle>(10,20)
             │                            │
             │                      unique_ptr owns it
             │                            │
             └────────────┬───────────────┘
                          │
                  automatic cleanup
                          │
                         RAII


virtual     → enable dynamic dispatch (runtime choice)
override    → compiler checks you really overrode a base virtual
pure virtual (= 0) → class is abstract; you cannot create Shape objects
virtual dtor → safe delete through base pointer

Need a concrete Rectangle?
    auto r = make_unique<Rectangle>(10, 20);

Need "some Shape" (runtime polymorphism)?
    unique_ptr<Shape> s = make_unique<Rectangle>(10, 20);
*/
