#ifndef BUZZCTRL_H
#define BUZZCTRL_H

#include "CppThread.h"
#include "client.h"
#include <iostream>
#include <pigpio.h> 
#include <cstring>

// BuzzCtrl class inherits from CppThread to handle buzzer control based on client signals
class BuzzCtrl : public CppThread {
private:
    Client& clie;  // Reference to a Client object

public:
    // Constructor that initializes the BuzzCtrl object with a reference to a Client object
    BuzzCtrl(Client& Clie) : clie(Clie) {}

    // Overridden run method from CppThread to perform buzzer control in a loop
    void run() override {
        gpioSetMode(17, PI_OUTPUT);  // Set GPIO 17 as output for the buzzer

        while (true) {
            // Check if the received message is "R"
            if (strcmp(clie.rdmsg, "R") == 0) {
                std::cout << "Car Stopped!!!" << std::endl;
                gpioPWM(17, 128);  // Activate the buzzer
            } else if (clie.flag == 1) {
                gpioPWM(17, 0);  // Deactivate the buzzer
                std::cout << "no buzzzzzzzzzz\n";
            } else {
                std::cout << "Car Running" << std::endl;
                gpioPWM(17, 0);  // Deactivate the buzzer
                std::cout << "no buzzzzzzzzzz\n";
            }
        }
    }
};

#endif // BUZZCTRL_H
