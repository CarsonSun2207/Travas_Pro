#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Server.h"

// Client class represents a client that interacts with the server.
// It can scan cards and send messages to the server.
class Client {
public:
    // Constructor initializes the server reference and sets the flag to 0
    Client(Server* server) : server_(server), flag(0) {
        std::cout << "Client constructor" << std::endl;
    }

    // Virtual destructor
    virtual ~Client() {
        std::cout << "Client destructor" << std::endl;
    }

    // Virtual method to scan a card with a given card ID
    virtual void scanCard(const std::string& cardID);

    // Virtual method to send a message to the server
    virtual void sendmsg();

    // Public flag variable used for determining the message to send
    int flag;

    // Getter for the server reference
    Server* getServer() const { return server_; }

private:
    Server* server_; // Pointer to the server instance
};

#endif // CLIENT_H
