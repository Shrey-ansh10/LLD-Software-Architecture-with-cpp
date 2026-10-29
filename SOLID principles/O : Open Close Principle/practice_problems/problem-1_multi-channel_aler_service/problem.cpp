// The Problem

// You are building a system monitoring service that notifies on-call engineers when a server goes down. Currently, the team has this code:

#include <iostream>
using namespace std;

enum class ChannelType {
    EMAIL,
    SMS,
    PUSH_NOTIFICATION
};

class AlertService {
public:
    void sendAlert(ChannelType channel, const std::string& recipient, const std::string& message) {
        if (channel == ChannelType::EMAIL) {
            std::cout << "Connecting to SMTP server...\n";
            std::cout << "Sending Email to " << recipient << ": " << message << "\n";
        } 
        else if (channel == ChannelType::SMS) {
            std::cout << "Connecting to Twilio Telecom Gateway...\n";
            std::cout << "Sending SMS to " << recipient << ": " << message << "\n";
        } 
        else if (channel == ChannelType::PUSH_NOTIFICATION) {
            std::cout << "Connecting to Apple APNS / Firebase FCM...\n";
            std::cout << "Sending Push Notification to device " << recipient << ": " << message << "\n";
        }
    }
};

/*

Your Task :
1. Identify the violation: What happens when the ops team wants to add SLACK_WEBHOOK or DISCORD?
    -  As seen the class AlertService has a method that's responsible for sending alert, but due to the way it's designed - using the if-else if chain, if future if this class needs to be extended with a new type of alert, the class code will be needed to be modified.

    - That is the direct violation of the OCP.

2. Refactor: Create an abstract interface (e.g. NotificationChannel) and concrete channel classes so that AlertService never needs to be modified when new communication channels are added.


*/