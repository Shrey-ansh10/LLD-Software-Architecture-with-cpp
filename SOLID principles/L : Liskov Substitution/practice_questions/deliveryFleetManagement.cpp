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
    void move(){
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
        if(litersAdded < maxFuelTankCapacity && 
            litersAdded > 0 && 
            litersAdded <= maxFuelTankCapacity - currentFuelLevel){
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

// test refuel mechanism
void prepMotorizedVehicles(const vector<FuelVehicle*>& motorizedFleet, const vector<double> amount){
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

    // `.get()` gives a non-owning Vehicle* pointer, while unique_ptr still handles cleanup!
    vector<Vehicle*> allVehicles;
    allVehicles.push_back(bicycle.get());
    allVehicles.push_back(motorBike.get());
    allVehicles.push_back(eBike.get());

    deployAllVehicles(allVehicles);

    vector<FuelVehicle*> fuelBasedVehicle = {motorBike.get(), eBike.get()};

    prepMotorizedVehicles(fuelBasedVehicle,{10, 15});
}