I'm defining the things in the very basics for now.

Class : A blueprint / user-defined data type that defines the data and behavior its objects can have.
A combination of Data + Functions.

Object : An instance of the class.

Methods : Functions defined in the class. In C++ they are called member functions.
- Define the functionality / behavior of the object.

Data / Attributes : The data of the object (data members).
- Variables defined in a class that represent the state associated with an object.
- Normally, each object has its own copy of non-static data members.

Basics of OOP:

- Encapsulation : Bundling the data/state and the functions that operate on that data inside a class,
  while controlling access to the internal state.
  In C++ this is done with access specifiers: private, protected, public.

- Inheritance : A class can reuse and extend another class. This is typically an "is-a" relationship.
  A child class inherits attributes and methods of its parent (subject to access rules).

## Polymorphism :
"Many forms."

It allows the same interface / function name to represent different behavior depending on the type or arguments involved.

In C++, this can happen at compile time (overloading, templates) or at runtime (inheritance + virtual functions).

Two important forms in C++:

1. Compile-time / Static Polymorphism
   - Function overloading
   - Operator overloading
   - Templates

2. Runtime / Dynamic Polymorphism
   - Function overriding using virtual functions

# Abstraction :
Exposing the necessary functionality / interface while hiding unnecessary implementation details.
The user of an object should know WHAT it can do without necessarily knowing HOW it internally does it.

Abstract classes / interfaces are one way of creating abstractions, but abstraction does not require an abstract class.

# Composition :
Building a class using objects of other classes as its components.
It represents a "has-a" relationship. Example: a Car has an Engine.


Other OOP concepts:

TYPE OF RELATIONSHIP:
1. "Has a"
   Car has an Engine
   → composition / aggregation / association

2. "Is a"
   Dog is an Animal
   → typically inheritance


CARDINALITY / MULTIPLICITY

1. One-to-One
   Person ───── Passport

2. One-to-Many
   Department ───── Employees

3. Many-to-Many
   Students ───── Courses

(Note: cardinality terms are particularly common in database modeling and UML.
They are not themselves fundamental OOP principles.)
