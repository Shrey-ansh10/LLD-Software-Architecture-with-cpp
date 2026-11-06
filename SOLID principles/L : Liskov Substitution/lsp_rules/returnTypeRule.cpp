#include <iostream>

using namespace std;

// Return Type Rule : 
// Subtype overriden method return type should be either identical 
// or narrower then the parent method's return type.
// This is also called as return type covariance.
// C++ enforces this by covariance.

class Animal {
public:
    virtual ~Animal() = default;
    //some common Animal methods
};

class Dog : public Animal {
    //Additional Dog methods specific to Dogs.
};


class Parent {
public:
    virtual ~Parent() = default;

    virtual Animal* getAnimal() { 
        cout << "Parent : Returning Animal instance" << endl;
        return new Animal();
    }
};

class Child : public Parent {
public:
//  Can also have return type as Dog* (C++ directly supports Covariant Return Types!)
    Dog* getAnimal() override { 
        cout << "Child : Returning Dog instance (narrower/covariant return type Dog*)" << std::endl;
        return new Dog();
    }
};

class Client {
private:
    Parent* p;

public:
    Client(Parent* p) {
        this->p = p;
    }
    void takeAnimal() {
        Animal* a = p->getAnimal();
        delete a; // Clean up allocated Animal/Dog
    }
};

int main() {
    Parent* parent = new Parent();
    Child* child = new Child();

    Client* client = new Client(child);
    //Client * client = new Client(parent);
    client->takeAnimal();

    delete client;
    delete child;
    delete parent;

    return 0;
}
