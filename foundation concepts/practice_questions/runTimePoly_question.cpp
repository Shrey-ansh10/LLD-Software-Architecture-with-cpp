/*
    Q5 — Runtime polymorphism + abstraction
    Build: abstract Notifier with
    virtual void send(const string& message) const = 0
    and virtual ~Notifier() = default

    Derived:

    EmailNotifier — prints email: <message>
    SmsNotifier — prints sms: <message>
    Client: void alert(const Notifier& n, const string& msg)
    Inside alert, you may only use Notifier. No EmailNotifier / SmsNotifier names.

    main:


    alert(email, "otp 1234");
    alert(sms, "otp 1234");
    Also: Notifier n; must not compile.

    Done when: adding PushNotifier later would not change alert. That is abstraction.

    Extra (readiness check): write void bad(Notifier n); and explain why that is wrong even if it compiled (slicing). Use const Notifier& or unique_ptr<Notifier> instead.
*/

