#include <iostream>
#include <string>
using namespace std;

// The ChannelType class becomes irrelevant in this design.

// =================================
// ABSTRACTION (The Contract)
// =================================
class NotificationChannel {
public:
    virtual void sendNotification(const std::string& recipient, const std::string& message) const = 0;
    virtual ~NotificationChannel() = default;
};

class EmailNotification : public NotificationChannel {
public:
    void sendNotification(const string &recipient, const string &message) const override {
        cout << "Connecting to SMTP server...\n";
        cout << "Sending Email to " << recipient << ": " << message << "\n";
    }
};

class SMSNotification : public NotificationChannel {
public:
    void sendNotification(const std::string& recipientName, const string &message) const override {
        cout << "Connecting to Twilio Telecom Gateway...\n";
        cout << "Sending SMS to " << recipientName << ": " << message << "\n";
    }
};

class PushNotification : public NotificationChannel {
public:
    void sendNotification(const string &recipientDevice, const string &message) const override {
        cout << "Connecting to Apple APNS / Firebase FCM...\n";
        cout << "Sending Push Notification to device " << recipientDevice << ": " << message << "\n";
    }
};

class SlackNotification : public NotificationChannel {
public:
    void sendNotification(const string &recipientSlackID, const string &message) const override {
        cout << "Connecting to the Slack webhook ... \n" ;
        cout << "Sending Slack notification to " << recipientSlackID << ": " << message << endl;
    }
};

class DiscordNotification : public NotificationChannel {
public:
    void sendNotification(const string &recipientDiscordID, const string &message) const override {
        cout << "Connecting to discord server..." << endl;
        cout << "Sending message to " << recipientDiscordID << ":" << message << endl;
    }
};

// ==================================================
// THE Notification Channel as mentioned ==================================================
class AlertService{
public:
    void sendAlert(const NotificationChannel &alertType, const string &recipient, const string &message){
        alertType.sendNotification(recipient, message);
        cout << "Notification sent to " << recipient << endl;
    }
};




int main(){

    AlertService alert;

    EmailNotification email;
    alert.sendAlert(email, "receiver.to.sender@email.com", "Message for email notification");
    
    SMSNotification sms;
    alert.sendAlert(sms, "Harry", "This is very important message. Just Kidding!");

    PushNotification push;
    alert.sendAlert(push, "mobile phone", "Your screen time is INSANE! Take some rest.");
    
    SlackNotification slack;
    alert.sendAlert(slack, "harry_puttar101", "What do you use Slack and not WhatsAPP?");
    
    DiscordNotification disc;
    alert.sendAlert(disc, "101harry_paji_rocks", "Hey this is discord? WOW, that's cool!");

    return 0;
}