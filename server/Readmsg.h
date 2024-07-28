#ifndef READMSG_H
#define READMSG_H

#include "CppThread.h"
#include "Server.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

// Readmsg class inherits from CppThread to handle message reading in a separate thread.
// It continuously checks for new messages from the server and prints them to the console.
class Readmsg : public CppThread {
private:
    Server& ser; // Reference to the server object
    // Overridden run method that contains the thread's execution code
    void run()  {
        while (true) {
            // Check if there are bytes to read from the server
            if (ser.byteno() != 0) {
                // Copy the message read from the server into rdmsg and print it
                strcpy(rdmsg, ser.readctmsg());
                std::cout << rdmsg << std::endl;
                // std::this_thread::sleep_for(200ms); // Uncomment to add a delay
            }
        }
    }

public:
    // Constructor initializing the server reference
    Readmsg(Server& serversig) : ser(serversig) {};
    bool trigger; // Trigger flag (purpose to be defined in context)
    char rdmsg[9] = {0}; // Buffer to store the read message
};

#endif // READMSG_H
