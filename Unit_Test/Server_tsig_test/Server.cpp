// Server.cpp
#include "Server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

int Server::byteno() {
    return read(clt_soc, bff, 8);
}

void Server::setupServer(int& server_fd, struct sockaddr_in& address, int port) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cout << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    } else if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) != 0) {
        std::cout << "Socket creation succeed!!" << std::endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    std::cout << "After socket creation" << std::endl;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    } else if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) >= 0) {
        std::cout << "Bind success" << std::endl;
    }

    if (listen(server_fd, 3) < 0) {
        std::cout << "Listen" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::acceptConnection(int server_fd, struct sockaddr_in& address) {
    int addrlen = sizeof(address);
    clt_soc = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    std::cout << "after socket accept" << endl;
    if (clt_soc > 0) {
        std::cout << "Accept" << std::endl;
    }
}

char* Server::readctmsg() {
    char* buffer = new char[bffsize];
        memset(buffer, 0, bffsize);
        ssize_t bytesRead = read(clt_soc,buffer,bffsize);
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
void Server::sendtrafsig(bool tfsig){
    char tfmsg[2]={0} ;
    if (tfsig==0)
    {
        strcpy(tfmsg, "R");
         write(clt_soc, tfmsg, 2);
    }else
    { 
        strcpy(tfmsg, "G");
        write(clt_soc, tfmsg, 2);

    }
       
 }
