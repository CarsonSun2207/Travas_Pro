#include "client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Constructor implementation for Client class
Client::Client() {
    // Empty constructor
}

// Destructor to free allocated memory
Client::~Client() {
    delete[] rdmsg;
}

// Method to read bytes from the socket
int Client::byte_rd() {
    return read(sock, rdmsg, 5);
}

// Method to set up the socket connection
void Client::setupSocket(const std::string& host, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);  // Create a socket
    if (sock < 0) {
        std::cout << "Socket creation error " << std::endl;
        return;
    }

    struct sockaddr_in serv_addr;            // Server address structure
    serv_addr.sin_family = AF_INET;          // Address family
    serv_addr.sin_port = htons(port);        // Port number

    // Convert host IP address from text to binary form
    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported " << std::endl;
        return;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed " << std::endl;
    } else {
        std::cout << "Connected" << std::endl;
    }
}

// Method to send a message through the socket
void Client::sendmsg() {
    if (flag == 0) {
        std::cout << entermsg << std::endl;  // Print enter message
        send(sock, entermsg, strlen(entermsg), 0);  // Send enter message
    } else {
        std::cout << exitmsg << std::endl;  // Print exit message
        send(sock, exitmsg, strlen(exitmsg), 0);  // Send exit message
    }
}

// Method to identify signal from the received message
bool Client::sigiden() {
    if (strcmp(rdmsg, "RED") == 0) {
        sig = 0;  // Set signal to 0 for "RED"
    } else if (strcmp(rdmsg, "GREEN") == 0) {
        sig = 1;  // Set signal to 1 for "GREEN"
    }
    return sig;
}

// Method to read and process the signal from the socket
void Client::readsig() {
    memset(rdmsg, 0, 2);  // Clear the message buffer
    bytesRead = read(sock, rdmsg, 2);  // Read 2 bytes from the socket
    read(sock, rdmsg, 2);  // Read another 2 bytes from the socket (this seems redundant)
    std::cout << rdmsg << std::endl;  // Print the received message
    sigiden();  // Identify the signal from the message

    // Error handling for reading from the socket
    if (bytesRead < 0) {
        std::cerr << "Error reading from socket" << std::endl;
    } else if (bytesRead == 0) {
        std::cerr << "Connection closed by peer" << std::endl;
    }
}
