#ifndef TRAFFICSIG_H
#define TRAFFICSIG_H

#include "CppThread.h"
#include "Readmsg.h"
#include "Server.h"
#include <iostream>

// Trafficsig class inherits from CppThread to handle traffic signal control in a separate thread.
// It manages traffic signals based on messages read by Readmsg and interacts with the Server to send signals.
class Trafficsig : public CppThread {
private:
    Readmsg& rdmsg;          // Reference to the Readmsg instance
    Server& ser;             // Reference to the Server instance
    bool traffic_signal;     // Current state of the traffic signal
    int count = 0;           // Counter for internal use
    const int timer_id = 0;  // Timer ID for scheduling traffic signal changes

    // Static function to handle traffic signals, called by the timer
    static void staticTrafficSignalHandler(void* userdata) {
        // Cast userdata to Trafficsig instance and call the member function
        Trafficsig* instance = static_cast<Trafficsig*>(userdata);
        instance->trafficSignalHandler();
    }

    // Member function to handle traffic signals
    void trafficSignalHandler() {
        ser.sendtrafsig(traffic_signal);  // Send the current traffic signal state
        traffic_signal = !traffic_signal; // Toggle the traffic signal state
        ser.sendtrafsig(traffic_signal);  // Send the new traffic signal state
        std::cout << "The traffic signal is " << traffic_signal << std::endl;
    }

    // Overridden run method that contains the thread's execution code
    void run() {
        traffic_signal = 0; // Initialize the traffic signal state
        while (1) {
            if (strcmp(rdmsg.rdmsg, "Entered") == 0) {
                for (int i = 0; i < 2; i++) {
                    gpioSetTimerFuncEx(timer_id, 5000, staticTrafficSignalHandler, this);
                }
            } else if (strcmp(rdmsg.rdmsg, "Exited") == 0) {
                traffic_signal = 0;
                std::cout << "The traffic signal is " << traffic_signal << std::endl;
            }
        }
    }

public:
    // Constructor initializing references to Readmsg and Server instances and setting initial traffic signal state
    Trafficsig(Readmsg& rdmsg, Server& server) : rdmsg(rdmsg), ser(server), traffic_signal(false) {}
};

#endif // TRAFFICSIG_H
