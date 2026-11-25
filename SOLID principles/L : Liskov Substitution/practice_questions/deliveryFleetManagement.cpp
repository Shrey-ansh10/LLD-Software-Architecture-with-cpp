/*
Problem 3: End-to-End Design — Delivery Fleet Management (Medium–Hard)

The Scenario : A food-delivery startup manages a fleet of vehicles: Petrol Motorcycle, Electric Scooter, and Delivery Bicycle. Initially, a developer created a single Vehicle base class with:

- void move()
- void startEngine()
- void refuelPetrol(double liters)

Soon, problems appeared:
- Bicycle doesn't have an engine (startEngine() makes no sense) and doesn't use fuel!
- ElectricScooter has a motor (startEngine() / power on works), but it charges a battery (chargeBattery(double kWh)), not petrol!


Your Task :
Design and implement an LSP-compliant hierarchy (or composition + abstractions) in C++ for this fleet:

1. Any vehicle can move() (so a fleet tracker can move all vehicles polymorphically).
2. Motorized vehicles (PetrolMotorcycle, ElectricScooter) can start their engine/motor, while Bicycle is never forced to implement startEngine().
3. Energy replenishment is handled cleanly without any vehicle throwing "Unsupported fuel type" exceptions.
4. Write a main() demonstrating fleet operations (deployAllVehicles, etc.).

*/


#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Vehicle {
public:
    // All vehicles will move - so this is a core implementation
    // We can make it virtual in case the company wants to add drone or helicopetr in future, OR A drived class of this can be made for FlyingVehicles - depends on the architect's choice
    virtual void move() = 0;

    virtual ~Vehicle() = default;
};

class Bicycle : public Vehicle {
public:
    void move() override{
        cout << "Move delivery bicycle" << endl;
    }
};

class FuelVehicle : public Vehicle {
public:
    virtual double getVehicleFuelLevel() = 0;
    virtual double refuelVehicle(double amount) = 0; // will work as per the vehicle type - electric can implement it's refuel/charging type, and petrol can do it's own

    void startVehicle(){ // this stays same for both electric and petrol and also if we add diesel vehicle in future
        // turns vehicle on
        cout << "Vehicle Started." << endl;
    }
};

class PetrolMotorcycle : public FuelVehicle {
double currentFuelLevel; 
double maxFuelTankCapacity; // liters - pre-set for Motorcycles

public:
    PetrolMotorcycle(double initialFuelLevel, double maxFuelTankCapacity) : currentFuelLevel(initialFuelLevel), maxFuelTankCapacity(maxFuelTankCapacity) {}

    double getVehicleFuelLevel() override{
        return currentFuelLevel;
    }

    void move() override{
        cout << "Moves Motercycle." << endl;
    }

    double refuelVehicle(double litersAdded) override {
        // fills petrol
        if(litersAdded > 0 && 
            litersAdded + currentFuelLevel <= maxFuelTankCapacity){
            currentFuelLevel += litersAdded;         
        }
        return currentFuelLevel;
    }
};

class ElectricScooter : public FuelVehicle {
double batteryLevel;

public:
    ElectricScooter(double initialBatterLevel) : batteryLevel(initialBatterLevel) {}

    double getVehicleFuelLevel() override{
        return batteryLevel;
    }

    void move() override{
        cout << "Moves Electric Scooter" << endl;
    }

    double refuelVehicle(double inputChargePercent) override{
        if(inputChargePercent < 100 && 
            inputChargePercent > 0 && 
            inputChargePercent <= 100 - batteryLevel){
            batteryLevel += inputChargePercent;         
        }
        return batteryLevel;
    }
};

// test deploying - i.e. moving the vehicle
void deployAllVehicles(const vector<Vehicle*>& fleet){
    for(Vehicle* v : fleet){
        v->move();
    }
}

/*
NOTE : "std::vector" Cannot Hold References (vector<Vehicle&>)
   
- In C++, references (&) are aliases, not reassignable objects in memory—so vector<Vehicle&> is illegal in C++.

*/


// test refuel mechanism
void prepMotorizedVehicles(const vector<FuelVehicle*>& motorizedFleet, const vector<double>& amount){
    if(motorizedFleet.size() != amount.size()) return;

    int length = motorizedFleet.size();

    for(int i=0; i<length; i++){
        cout << "Vehicle Fuel-level Before refueling : " << motorizedFleet[i]->getVehicleFuelLevel() << endl;
        cout << "Vehicle Fuel Level After refueling :" <<  motorizedFleet[i]->refuelVehicle(amount[i]) << endl;
    }
    
    
    // for(FuelVehicle* v : motorizedFleet){
    //     cout << "Vehicle Fuel-level Before refueling : " << v->getVehicleFuelLevel() << endl;
    //     for(double amt : amount){
    //         cout << "Vehicle Fuel Level After refueling :" <<  v->refuelVehicle(amt) << endl;
    //     }
    // }
}

int main(){

    auto bicycle = make_unique<Bicycle>();
    auto motorBike = make_unique<PetrolMotorcycle>(10, 25);
    auto eBike = make_unique<ElectricScooter>(80);

    // `.get()` gives a non-owning 'Vehicle*' pointer (aka OBSERVER POINTER), while 'unique_ptr' still handles cleanup!
    vector<Vehicle*> allVehicles;
    allVehicles.push_back(bicycle.get()); // so we can't pass bicycle pointer itself here, cos once the function is over, this pointer is also deleted.
    allVehicles.push_back(motorBike.get()); // Therefor to avoid that, we pass in the OBSERVER POINTER which we get using '.get()' method on unique pointer
    allVehicles.push_back(eBike.get());

    deployAllVehicles(allVehicles);

    vector<FuelVehicle*> fuelBasedVehicle = {motorBike.get(), eBike.get()};

    prepMotorizedVehicles(fuelBasedVehicle,{10, 15});
}



/*
=====================================================================================================
Problem 4: Trade-off & Earlier Concept Check — Deep Hierarchies vs. Composition
=====================================================================================================

The Scenario :
To fix LSP violations, developers often keep splitting base classes into deeper inheritance trees
(e.g., Vehicle → MotorizedVehicle → FuelMotorizedVehicle → ...).

Your Task (Conceptual / Short Note)

-----------------------------------------------------------------------------------------------------
Q1. What happens when a new type comes along that cuts across two branches
    (e.g., a Hybrid Car that uses both petrol and battery, or an E-Bike that has pedals and a battery)?
-----------------------------------------------------------------------------------------------------

Ans: Let's address each one of them respectively.

(a) E-Bike :
    - With the current code, an E-Bike can simply inherit from FuelVehicle and implement its own
      properties as well (battery + pedals).
    - But notice: an E-Bike genuinely belongs to TWO branches — the Bicycle/pedal branch AND the
      electric branch. Inheriting from only one means re-writing (duplicating) the other branch's
      behavior inside EBike.

(b) Hybrid Car :
    Option 1 (with the current structure):
    - The HybridCar class can inherit directly from FuelVehicle and define its own behavior.

    Option 2 (a more structured / hierarchical tree):

        Vehicle -> FueledVehicle -> ElectricFuelVehicle -> ElectricBike, E-Scooter, E-Truck, E-Aeroplane etc.
                        |
                        |-> GasFueledVehicle    -> PetrolMotorcycle, PetrolCar, PetrolBike
                        |
                        |-> DieselFueledVehicle -> DieselTruck, DieselHelicopter

    - A HybridCar would then need multiple inheritance from ElectricFuelVehicle and GasFueledVehicle.
    - It also gets messy for variants like plug-in vs. non-plug-in hybrids.
    - Option 1 is simpler for now, but BOTH options run into the two classic problems below.

(c) The 2 classic problems with cross-cutting types in deep inheritance trees :

    Problem 1 : The Diamond Problem (C++ Multiple Inheritance)
    - If HybridCar inherits from both ElectricFuelVehicle and GasFueledVehicle, it gets two ambiguous
      copies of FueledVehicle (and Vehicle):

                      FueledVehicle
                      /           \
          ElectricFuelVehicle    GasFueledVehicle
                      \           /
                        HybridCar      <-- Which copy of FueledVehicle::startVehicle() does it call?!

    Problem 2 : Class Explosion (Combinatorial Explosion) & Code Duplication
    - Every time a vehicle combines capabilities from different branches (Pedal + Electric,
      Petrol + Electric, Diesel + Electric, ...), a strict inheritance tree forces you to either
      duplicate code across branches or keep creating new combination subclasses.

    => Conclusion: Splitting base classes deeper and deeper to satisfy LSP does not scale once
       capabilities start to mix.

-----------------------------------------------------------------------------------------------------
Q2. Using what you learned in foundation concepts/composition.cpp, how can Composition ("has-a") help
    avoid deep inheritance trees while still keeping your design LSP- and OCP-compliant?
-----------------------------------------------------------------------------------------------------

Ans:
(a) The core idea (my intuition) :
    - Composition, as far as I feel, is the most beautiful thing. All real objects are made of
      components, so we can use components to build broader classes, which are directly used to make
      final objects.
    - This is better than creating objects with everything predefined via inheritance, and then
      realizing that some object doesn't have one property — which takes the whole inheritance tree
      to hell.

(b) Example — Hybrid Car :
    - A hybrid car will definitely have all the basic properties of a Vehicle: it moves, it brakes,
      it accelerates.
    - What differs is the power source. So we can have a broader Engine class and a Battery class.
    - We build the engine as per our custom needs, pick a battery as per our needs, and then construct
      the HybridCar using these objects as components inside the HybridCar class.

        Vehicle  ──has-a──►  [ Engine ]                         (PetrolMotorcycle)
        Vehicle  ──has-a──►  [ Battery ]                        (ElectricScooter)
        Vehicle  ──has-a──►  [ PedalDrive ]                     (Bicycle)
        Vehicle  ──has-a──►  [ PedalDrive ] + [ Battery ]       (E-Bike)
        Vehicle  ──has-a──►  [ Engine ] + [ Battery ]           (HybridCar)

(c) How this keeps the design LSP- and OCP-compliant :
    - Instead of a deep Vehicle inheritance tree, a Vehicle HAS-A propulsion/energy component
      (Engine, Battery, PedalDrive — or a list of them for a HybridCar / E-Bike).
    - LSP is preserved : every Vehicle genuinely supports move() without dummy / throwing methods.
    - OCP is preserved : a new type (HybridCar, EBike, ...) is assembled by combining existing
      components — no existing vehicle class needs to be modified.

This is kind of what I was thinking about when I was implementing the deliveryFleetManagement program.
*/