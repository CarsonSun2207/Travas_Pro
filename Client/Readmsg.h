#ifndef READMSG_H
#define READMSG_H

#include "CppThread.h"
#include "client.h"
#include <iostream>
#include <pigpio.h> // Assuming you're using the pigpio library for gpioDelay

// Class Readingmsg inherits from CppThread and handles reading messages from a Client object
class Readingmsg : public CppThread {
private:
    Client& clie; // Reference to a Client object

public:
    // Constructor that initializes the Readingmsg object with a reference to a Client object
    Readingmsg(Client& Clie) : clie(Clie) {}

    // Overridden run method from CppThread to perform the reading operation in a loop
    void run() override {
        while (true) {
            // Check if there are bytes to read from the client
            if (clie.byte_rd() != 0) {
                clie.readsig(); // Read and process the signal
            }
        }
    }
};

#endif // READINGMSG_H
