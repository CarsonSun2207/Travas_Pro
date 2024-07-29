#ifndef CLIENT_H
#define CLIENT_H

#include <string>

// Client class to handle network communication
class Client {
private:
    bool sig = 1;                   // Signal status
    const char entermsg[9] = "Entered";  // Message for entering
    const char exitmsg[7] = "Exited";    // Message for exiting

public:
    char* rdmsg = new char[2];      // Buffer for reading messages
    int sock;                       // Socket descriptor
    bool flag;                      // Flag for signal status
    int bytesRead;                  // Number of bytes read from socket

    // Constructor
    Client();

    // Destructor to clean up allocated memory
    ~Client();

    // Method to read bytes from socket
    int byte_rd();

    // Method to set up socket connection
    void setupSocket(const std::string& host, int port);

    // Method to send a message through the socket
    void sendmsg();

    // Method to identify signal from the received message
    bool sigiden();

    // Method to read and process the signal from the socket
    void readsig();
};

#endif // CLIENT_H
