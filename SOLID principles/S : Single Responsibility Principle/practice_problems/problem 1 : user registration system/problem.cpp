// User Registration System
// The Scenario : You are building an authentication module. Currently, a
// developer created a monolithic UserManager class

#include <iostream>
using namespace std;

class UserManager {
private:
  std::string username;
  std::string email;
  std::string password;

public:
  UserManager(string u, string e, string p)
      : username(u), email(e), password(p) {}

  // 1. Validation logic
  bool validateEmail() { /* checks if email contains '@' and valid domain */ }
  bool validatePassword() { /* checks password length >= 8 and special chars */
  }

  // 2. Data persistence
  void saveToDatabase() { /* connects to DB, runs SQL: INSERT INTO users ... */
  }

  // 3. Notification
  void sendWelcomeEmail() { /* connects to SMTP server and sends welcome mail */
  }
};

/*
What’s Wrong? (Code Smells)
- If password rules change (e.g., minimum 10 characters), you touch UserManager.
- If database schema changes or you switch from MySQL to MongoDB, you touch
UserManager.
- If email template changes or you switch from Email to SMS/WhatsApp, you touch
UserManager.


Your Goal
Refactor this into clean, decoupled C++ classes where:

1. The User is purely a domain entity/data model.
2. A separate Validator handles input validation rules.
3. A separate Repository handles database persistence.
4. A separate Email/Notification Service handles sending messages.
*/