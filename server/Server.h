// Server.h
#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

// Server class handles server-side operations including setting up the server,
// accepting client connections, reading messages from the client, and sending traffic signals.
class Server {
private:
    char bff[8] = {0}; // Buffer to hold data read from the client
    int bffsize = 8;   // Size of the buffer

public:
    int clt_soc; // Client socket file descriptor

    // Reads the number of bytes from the client socket
    int byteno();

    // Sets up the server with the provided file descriptor, address, and port
    void setupServer(int& server_fd, struct sockaddr_in& address, int port);

    // Accepts a connection from a client
    void acceptConnection(int server_fd, struct sockaddr_in& address);

    // Reads a message from the client socket
    char* readctmsg();

    // Sends a traffic signal to the client
    void sendtrafsig(bool tfsig);
};

#endif // SERVER_H
