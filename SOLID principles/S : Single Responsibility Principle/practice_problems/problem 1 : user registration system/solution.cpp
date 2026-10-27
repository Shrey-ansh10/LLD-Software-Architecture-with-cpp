/*
So following are the things we need to implement

1. The User is purely a domain entity/data model. - we can create a user Model -
will only contain data

2. A separate Validator handles input validation rules. - ok

3. A separate Repository handles database persistence. - okay

4. A separate Email/Notification Service handles sending messages. - alright
*/

#include <iostream>
#include <string>
using namespace std;

// =====================
// 1. Domain Entity
// =====================
class User {
  std::string username;
  std::string email;
  std::string password;

public:
  User(const std::string &username, const std::string &email,
       const std::string &password)
      : username(username), email(email), password(password) {}

  std::string getUserEmail() const { return email; }

  std::string getUsername() const { return username; }

  std::string getPassword() const { return password; }
};

// =====================
// 2.  Validator
// =====================
class Validator {
  // std::string email;
  // std::string password;

public:
  virtual bool validate(const std::string &s) const = 0; // validate
  virtual ~Validator() = default;
};

class EmailValidator : public Validator {
public:
  bool validate(const std::string &eml) const override {
    return eml.find('@') != string::npos;
  }
};

class PasswordValidator : public Validator {
public:
  bool validate(const std::string &passwd) const override {
    return passwd.length() >= 10;
  }
};

// ======================
// 3. Data Persistence
// ======================
class DataPersistance {
public:
  virtual bool saveToDatabase(const User &user) const = 0;
  virtual ~DataPersistance() = default;
};

class SaveToSql : public DataPersistance {
public:
  bool saveToDatabase(const User &user) const override {
    bool saved = false;
    // SQL query to save to SQL DB
    // if save sucessfull - saved = true;
    return saved;
  }
};

// following type of DB's can later be added
// class SaveToMongo : public DataPersistance {
// public:
//     bool saveToDatabase(const User &user) const override {
//         // Query to save to mongo DB
//     }
// };

// ==========================
// 4. Notification Service
// ==========================
class Notifier {
public:
  virtual bool sendNotification(const User &user,
                                const std::string &message) const = 0;
  virtual ~Notifier() = default;
};

class EmailNotifier : public Notifier {
public:
  bool sendNotification(const User &user,
                        const std::string &message) const override {
    std::string email = user.getUserEmail();
    bool notificationSent = false;
    // send email logic
    // if sent sucessfull -> notificationSent = true;
    return notificationSent;
  }
};

// other types of notifier can be create using the Notifier class

// ==========================================
// 5. Orchestrator (Single Responsibility: User Registration Flow)
// ==========================================
class UserManager {

  // all data members will be of abstract class, so that any type of derived
  // class can be passed.
  const Validator &emlValidator;  // email validator
  const Validator &passValidator; // passwordvalidator
  const DataPersistance &where;
  const Notifier &notifier;

public:
  UserManager(const Validator &eml, const Validator &pass,
              const DataPersistance &type, const Notifier &n)
      : emlValidator(eml), passValidator(pass), where(type), notifier(n) {}

  bool registerUser(const User &user) {

    // 1. Validate Email
    if (!emlValidator.validate(user.getUserEmail())) {
      cout << "Email verification failed." << endl;
      return false;
    }

    // 2. Validate Password
    if (!passValidator.validate(user.getPassword())) {
      cout << " Password verification failed. Try Again! ";
      return false;
    }

    // 3. Save to DB
    if (!where.saveToDatabase(user)) {
      cout << "Can't save to DB. Save Failed. Please try again !" << endl;
      return false;
    }

    notifier.sendNotification(user, "Registration Successfull. Congrats !")
        ? cout << "User Registered and notification sent sucessfully." << endl
        : cout << "User Registered. Notification sent failed." << endl;

    return true;
  }
};

int main() {

  // created user
  User u1("Harry", "harry@gmail.com", "#MypassWORD");

  const EmailValidator ev;
  const PasswordValidator pv;
  const SaveToSql sql;           // tell where you wanna save it
  const EmailNotifier emlNotify; // tell how you wanna notify them

  UserManager manager(ev, pv, sql, emlNotify);
  manager.registerUser(u1);

  return 0;
}