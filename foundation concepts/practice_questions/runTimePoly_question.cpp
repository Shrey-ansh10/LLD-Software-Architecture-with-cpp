/*
    Q5 — Runtime polymorphism + abstraction
    Build: abstract Notifier with
    virtual void send(const string& message) const = 0
    and virtual ~Notifier() = default

    Derived:
    EmailNotifier — prints email: <message>
    SmsNotifier — prints sms: <message>
    
    Client(regular free function): void alert(const Notifier& n, const string& msg)
    
    Inside alert, you may only use Notifier. No EmailNotifier / SmsNotifier names.

    main:
    EmailNotifier email;
    SmsNotifier sms;
    alert(email, "otp 1234");
    alert(sms, "otp 1234");

    Also: Notifier n; must not compile, as it's a abstract class.

    Done when: adding PushNotifier later would not change alert. That is abstraction.

    Extra (readiness check): write void bad(Notifier n); and explain why that is wrong even if it compiled (slicing). Use const Notifier& or unique_ptr<Notifier> instead.
*/

#include <iostream>
using namespace std;

class Notifier{ // abstract class
public:
    virtual void send(const string& message) const = 0;
    virtual ~Notifier() = default;
};

class EmailNotifier : public Notifier {
public:
    void send(const string& message) const override {
        cout << "email: " << message < endl;
    }
};

class SmsNotifier : public Notifier {
public:
    void send(const string& message){
        cout << "sms: " << message << endl;
    }
};

void alert(const Notifier& n, const string& msg){
    n.send(msg);
}

int main(){

    // Notifier n; // will cause error as Notifier is a abstract class.
    
    EmailNotifier email;
    SmsNotifier sms;
    alert(email, "otp 1234");
    alert(sms, "otp 1234");
    
    return 0;
}