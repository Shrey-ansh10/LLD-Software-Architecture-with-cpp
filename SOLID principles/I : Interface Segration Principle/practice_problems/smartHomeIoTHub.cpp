/*
Problem 1: Implementer & Client Segregation — Smart Home IoT Hub (Easy)

The Scenario:
    - You are building a Smart Home controller. A developer wrote one universal ISmartDevice interface for every appliance in the house:

You need to support four devices:
    - SmartBulb: Can only turn on and off.
    - SmartThermostat: Turns on/off and adjusts temperature.
    - SmartSpeaker: Turns on/off and plays music.
    - SmartDoorLock: Turns on/off and locks the door.

Your Task :
1. Segregate the Interface: Split ISmartDevice into small, focused role interfaces (e.g., ISwitchable, ITemperatureControllable, IAudioPlayable, ILockable).
2. Implement the Devices: Ensure no device is forced to implement dummy/throwing methods (preserving LSP).
3. Write Client Functions:
    - void turnOffEntireHome(const vector<ISwitchable*>& devices) — powers off every device when the owner leaves.
    - void activateNightRoutine(ILockable& door, ITemperatureControllable& thermostat, ISwitchable& bedroomLight) — locks the door, sets temperature to 20.0°C, and turns off the light.

*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;


// Based on the description I can segrigate 4 functions of the devices:
// 1. Turn On and Off - We can implement a class called ISwitchable / TurnOnOff
// 2. Adjust Temperature - can implement a interface ITemperatureControllable
// 3. Play Music - IAudioPlayable
// 4. Can lock - ILocabale

class ISwitchable {
public:
    virtual ~ISwitchable() = default;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};

class ITemperatureControllable {
public:
    virtual ~ITemperatureControllable() = default;
    virtual void setTemperature(double celsius) = 0;
};

class IAudioPlayable {
public:
    virtual ~IAudioPlayable() = default;
    virtual void playMusic(const string& trackName) = 0;
};

class ILockable {
public:
    virtual ~ILockable() = default;
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

// ------------devices-------------

// You need to support four devices:
//     - SmartBulb: Can only turn on and off.
//     - SmartThermostat: Turns on/off and adjusts temperature.
//     - SmartSpeaker: Turns on/off and plays music.
//     - SmartDoorLock: Turns on/off and locks the door.


class SmartBulb : public ISwitchable {
    bool isOn;
public:
    SmartBulb() : isOn(false) {};

    void turnOn() override {
        isOn = true;
        cout << "[SmartBulb] Turned ON\n";
    }

    void turnOff() override {
        isOn = false;
        cout << "[SmartBulb] Turned OFF\n";
    }
};


class SmartThermostat : public ITemperatureControllable, public ISwitchable {
    double temperature;
    bool isOn;

public:
    SmartThermostat() : temperature(22), isOn(false) {};

    void turnOn() override {
        isOn = true;
        cout << "[SmartThermostat] Turned ON\n";
    }

    void turnOff() override {
        isOn = false;
        cout << "[SmartThermostat] Turned OFF\n";
    }

    void setTemperature(double celcius) override {
        temperature = celcius;
        cout << "[SmartThermostat] Temperature set to " << temperature << "°C\n";
    }
};

class SmartSpeaker : public ISwitchable, public IAudioPlayable {
    int volume;
    int maxVolume;
    bool isOn;
public:
    SmartSpeaker() : volume(5), maxVolume(15), isOn(false) {};

    void turnOn() override {
        isOn = true;
        cout << "[SmartSpeaker] Turned ON\n";
    }

    void turnOff() override {
        isOn = false;
        cout << "[SmartSpeaker] Turned OFF\n";
    }

    void playMusic(const string& trackName) override {
        cout << "[SmartSpeaker] Playing track: " << trackName << "\n";
    }
};


class SmartDoorLock : public ISwitchable, public ILockable {
    bool isOn;
    bool isLock;

public:
    SmartDoorLock() : isOn(false), isLock(false) {};

    void turnOn() override {
        isOn = true;
        cout << "[SmartDoorLock] Turned ON\n";
    }

    void turnOff() override {
        isOn = false;
        cout << "[SmartDoorLock] Turned OFF\n";
    }
    
    void lock() override{
        isLock = true;
        cout << "[SmartDoorLock] Door LOCKED\n";
    }

    void unlock() override {
        isLock = false;
        cout << "[SmartDoorLock] Door UNLOCKED\n";
    }
};

// --------client functions-----------------

// powers off every device when the owner leaves.
void turnOffEntireHome(const vector<ISwitchable*>& devices){
    for(ISwitchable* device : devices){
        if(device) device->turnOff();
    }
}

// locks the door, sets temperature to 20.0°C, and turns off the light.
void activateNightRoutine(ILockable& door, ITemperatureControllable& thermostat, ISwitchable& bedroomLight){
    door.lock();
    thermostat.setTemperature(20.00);
    bedroomLight.turnOff();
}

int main() {
    SmartBulb bedroomLight;
    SmartThermostat thermostat;
    SmartSpeaker livingRoomSpeaker;
    SmartDoorLock frontDoor;

    cout << "--- Powering On Devices & Playing Music ---\n";
    bedroomLight.turnOn();
    thermostat.turnOn();
    livingRoomSpeaker.turnOn();
    livingRoomSpeaker.playMusic("Lo-Fi Beats");
    frontDoor.turnOn();

    cout << "\n--- Activating Night Routine ---\n";
    activateNightRoutine(frontDoor, thermostat, bedroomLight);

    cout << "\n--- Turning Off Entire Home ---\n";
    vector<ISwitchable*> allDevices = {
        &bedroomLight,
        &thermostat,
        &livingRoomSpeaker,
        &frontDoor
    };
    turnOffEntireHome(allDevices);

    return 0;
}