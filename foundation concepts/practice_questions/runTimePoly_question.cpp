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
        cout << "email: " << message << endl;
    }
};

class SmsNotifier : public Notifier {
public:
    void send(const string& message) const override{
        cout << "sms: " << message << endl;
    }
};

// this addition should not break the code
class PushNotifier : public Notifier {
public:
    void send(const string& message) const override {
        cout << "push notification : " << message << endl;
    }
};

void alert(const Notifier& n, const string& msg){
    n.send(msg);
}

// Incorrent way - cos we're passing in the object by value - meaning it's copy will be created on stack in this function call. 
// As the Parameter tells the copy is just Notifier type and not Email or Sms notifier.
// So that cause a runtime error, if this function is called. Caused due to object slicing.
void badAlert(Notifier n, string msg){
    n.send(msg);
}
// EmailNotifier object
// ┌─────────────────────┐
// │ Notifier part       │  ← this is all `Notifier n` would keep
// │ + EmailNotifier part│  ← this gets chopped off (sliced)
// └─────────────────────┘
// So even if bad compiled, n inside it would not behave like email/SMS. Runtime polymorphism needs the real object still there. A reference keeps it there. A value copy does not.

int main(){

    // Notifier n; // will cause error as Notifier is a abstract class.
    
    EmailNotifier email;
    SmsNotifier sms;
    alert(email, "otp 1234");
    alert(sms, "otp 1234");
    
    PushNotifier push;
    alert(push, "otp 1234"); // works with new way of notifier added - abstraction

    return 0;
}