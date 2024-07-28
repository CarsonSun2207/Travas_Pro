// Server.cpp
#include "Server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

// Function to read the number of bytes from the client socket
int Server::byteno() {
    return read(clt_soc, bff, 8);
}

// Function to set up the server with the provided file descriptor, address, and port
void Server::setupServer(int& server_fd, struct sockaddr_in& address, int port) {
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cout << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    } else if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) != 0) {
        std::cout << "Socket creation succeed!!" << std::endl;
    }

    // Set up address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    std::cout << "After socket creation" << std::endl;

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    } else if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) >= 0) {
        std::cout << "Bind success" << std::endl;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cout << "Listen" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Function to accept a connection from a client
void Server::acceptConnection(int server_fd, struct sockaddr_in& address) {
    int addrlen = sizeof(address);
    clt_soc = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    std::cout << "after socket accept" << endl;
    if (clt_soc > 0) {
        std::cout << "Accept" << std::endl;
    }
}

// Function to read a message from the client socket
char* Server::readctmsg() {
    char* buffer = new char[bffsize];
    memset(buffer, 0, bffsize);
    ssize_t bytesRead = read(clt_soc, buffer, bffsize);
    
    // Check if read was successful
    if (bytesRead < 0) {
        // Error handling
        std::cerr << "Error reading from socket" << std::endl;
        delete[] buffer; // Free allocated memory
        return nullptr;
    } else if (bytesRead == 0) {
        // Connection closed
        std::cerr << "Connection closed by peer" << std::endl;
        delete[] buffer; // Free allocated memory
        return nullptr;
    } else {
        // Read successful
        return buffer;
    }
}

// Function to send a traffic signal to the client
void Server::sendtrafsig(bool tfsig) {
    char tfmsg[2] = {0};
    if (tfsig == 0) {
        strcpy(tfmsg, "R");
        write(clt_soc, tfmsg, 2);
    } else { 
        strcpy(tfmsg, "G");
        write(clt_soc, tfmsg, 2);
    }
}
