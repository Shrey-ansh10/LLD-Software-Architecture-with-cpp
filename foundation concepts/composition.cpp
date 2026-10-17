#include <iostream>
#include <string>
using namespace std;

// Composition: has-a. A Car HAS an Engine.
// Prefer this over inheritance when you want to use another class, not be a subtype of it.
// Car is not an Engine. Inheritance would be the wrong relationship.

class Engine {
    int horsepower;

public:
    Engine(int hp) : horsepower(hp) {}

    void start() const {
        cout << "Engine (" << horsepower << " hp) started\n";
    }
};

class Car {
    string model;
    Engine engine; // Car is built FROM an Engine object

public:
    Car(string m, int hp) : model(m), engine(hp) {}

    void start() const {
        cout << model << ": ";
        engine.start();
    }
};

int main() {
    Car car("Civic", 150);
    car.start();
    return 0;
}

/*
is-a  → inheritance   (Dog is an Animal)
has-a → composition   (Car has an Engine)

For LLD, composition is usually the better default.
You can change/replace the Engine without pretending Car is an Engine.
*/
