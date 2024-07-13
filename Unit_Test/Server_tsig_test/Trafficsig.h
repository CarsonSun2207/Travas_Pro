#ifndef TRAFFICSIG_H
#define TRAFFICSIG_H

#include "CppThread.h"
#include "Readmsg.h"
#include "Server.h"
#include <iostream>
#include "pigpio.h"

class TrafficsigTest;  // Forward declaration of the test class

class Trafficsig : public CppThread {
private:
    Readmsg& rdmsg;
    Server& ser;
    bool traffic_signal;
    int count = 0;
    const int timer_id = 0;

    void trafficSignalHandler() {
        ser.sendtrafsig(traffic_signal);
        std::cout << "The traffic signal is " << traffic_signal << std::endl;
        traffic_signal = !traffic_signal;
    }

    void run() {
        traffic_signal = 0;
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

    static void staticTrafficSignalHandler(void* userdata) {
        // Cast userdata to Trafficsig instance and call the member function
        Trafficsig* instance = static_cast<Trafficsig*>(userdata);
        instance->trafficSignalHandler();
    }

    friend class TrafficsigTest;  // Grant access to the test class

public:
    Trafficsig(Readmsg& rdmsg, Server& server) : rdmsg(rdmsg), ser(server), traffic_signal(false) {}
    
    void testStaticTrafficSignalHandler() {
        staticTrafficSignalHandler(this);
    }
    
    bool getTrafficSignal() const {
        return traffic_signal;
    }

};

#endif // TRAFFICSIG_H
