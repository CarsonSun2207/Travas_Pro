#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <iostream>

// Server class serves as an interface for handling server operations.
// It declares a pure virtual function that must be implemented by derived classes.
class Server {
public:
    // Constructor prints a message for debugging
    Server() {
        std::cout << "Server constructor" << std::endl;
    }
    
    // Virtual destructor prints a message for debugging
    virtual ~Server() {
        std::cout << "Server destructor" << std::endl;
    }
    
    // Pure virtual function to update the server with a card ID
    // This function must be overridden by any derived class.
    virtual void updateServerWithCardID(const std::string& cardID) = 0;
};

#endif // SERVER_H
